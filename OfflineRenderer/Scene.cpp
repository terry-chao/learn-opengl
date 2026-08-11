#include "Scene.h"

#include "Disk.h"
#include "Sphere.h"
#include "Triangle.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
struct XmlElement
{
    std::string name;
    std::string text;
    std::vector<XmlElement> children;

    const XmlElement* Child(const std::string& childName) const
    {
        for (const XmlElement& child : children)
        {
            if (child.name == childName)
            {
                return &child;
            }
        }
        return nullptr;
    }

    std::vector<const XmlElement*> Children(const std::string& childName) const
    {
        std::vector<const XmlElement*> result;
        for (const XmlElement& child : children)
        {
            if (child.name == childName)
            {
                result.push_back(&child);
            }
        }
        return result;
    }

    std::string ChildText(const std::string& childName, const std::string& fallback = {}) const
    {
        const XmlElement* child = Child(childName);
        return child ? child->text : fallback;
    }
};

std::string ReadFileText(const std::string& filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file)
    {
        return {};
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string text = ss.str();

    // 跳过 UTF-8 BOM，避免 <?xml> / <Scene> 解析失败
    if (text.size() >= 3
        && static_cast<unsigned char>(text[0]) == 0xEF
        && static_cast<unsigned char>(text[1]) == 0xBB
        && static_cast<unsigned char>(text[2]) == 0xBF)
    {
        text.erase(0, 3);
    }

    return text;
}

std::string StripXmlComments(std::string xml)
{
    while (true)
    {
        const size_t begin = xml.find("<!--");
        if (begin == std::string::npos)
        {
            break;
        }
        const size_t end = xml.find("-->", begin);
        if (end == std::string::npos)
        {
            xml.erase(begin);
            break;
        }
        xml.erase(begin, end + 3 - begin);
    }
    return xml;
}

void SkipWhitespace(const std::string& xml, size_t& i)
{
    while (i < xml.size() && std::isspace(static_cast<unsigned char>(xml[i])))
    {
        ++i;
    }
}

bool ParseXmlElement(const std::string& xml, size_t& i, XmlElement& out)
{
    SkipWhitespace(xml, i);
    if (i >= xml.size() || xml[i] != '<')
    {
        return false;
    }
    ++i;

    // Skip XML declaration / doctype
    if (i < xml.size() && (xml[i] == '?' || xml[i] == '!'))
    {
        const size_t close = xml.find('>', i);
        if (close == std::string::npos)
        {
            return false;
        }
        i = close + 1;
        return ParseXmlElement(xml, i, out);
    }

    const size_t nameBegin = i;
    while (i < xml.size() && !std::isspace(static_cast<unsigned char>(xml[i])) && xml[i] != '>' && xml[i] != '/')
    {
        ++i;
    }
    out.name.assign(xml, nameBegin, i - nameBegin);

    SkipWhitespace(xml, i);
    if (i < xml.size() && xml[i] == '/')
    {
        // <Tag/>
        ++i;
        if (i >= xml.size() || xml[i] != '>')
        {
            return false;
        }
        ++i;
        return true;
    }

    if (i >= xml.size() || xml[i] != '>')
    {
        return false;
    }
    ++i;

    const std::string closeTag = "</" + out.name + ">";
    while (i < xml.size())
    {
        SkipWhitespace(xml, i);
        if (i >= xml.size())
        {
            return false;
        }

        if (xml.compare(i, closeTag.size(), closeTag) == 0)
        {
            i += closeTag.size();
            // trim text
            size_t begin = 0;
            size_t end = out.text.size();
            while (begin < end && std::isspace(static_cast<unsigned char>(out.text[begin])))
            {
                ++begin;
            }
            while (end > begin && std::isspace(static_cast<unsigned char>(out.text[end - 1])))
            {
                --end;
            }
            out.text = out.text.substr(begin, end - begin);
            return true;
        }

        if (xml[i] == '<')
        {
            XmlElement child;
            if (!ParseXmlElement(xml, i, child))
            {
                return false;
            }
            out.children.push_back(std::move(child));
        }
        else
        {
            const size_t textBegin = i;
            while (i < xml.size() && xml[i] != '<')
            {
                ++i;
            }
            out.text.append(xml, textBegin, i - textBegin);
        }
    }

    return false;
}

float ParseFloat(const std::string& text, float fallback = 0.0f)
{
    if (text.empty())
    {
        return fallback;
    }
    try
    {
        return std::stof(text);
    }
    catch (...)
    {
        return fallback;
    }
}

Vector3f ParseVector3(const std::string& text)
{
    Vector3f result(0.0f);
    std::string normalized = text;
    for (char& c : normalized)
    {
        if (c == ',')
        {
            c = ' ';
        }
    }

    std::istringstream ss(normalized);
    ss >> result.x >> result.y >> result.z;
    return result;
}

Vector3f ParseDegreesVector3(const std::string& text)
{
    const Vector3f degrees = ParseVector3(text);
    return Vector3f(glm::radians(degrees.x), glm::radians(degrees.y), glm::radians(degrees.z));
}

void LoadPrimitives(SceneObject* sceneObject, const XmlElement& primitivesElement)
{
    for (const XmlElement& child : primitivesElement.children)
    {
        if (child.name == "Triangle")
        {
            const auto vertices = child.Children("Vertex");
            if (vertices.size() >= 3)
            {
                sceneObject->CreatePrimitive<Triangle>(
                    ParseVector3(vertices[0]->text),
                    ParseVector3(vertices[1]->text),
                    ParseVector3(vertices[2]->text));
            }
        }
        else if (child.name == "Sphere")
        {
            const float radius = ParseFloat(child.ChildText("Radius", "1"), 1.0f);
            sceneObject->CreatePrimitive<Sphere>(radius);
        }
        else if (child.name == "Disk")
        {
            const Vector3f normal = ParseVector3(child.ChildText("Normal", "0, 0, 1"));
            const float radius = ParseFloat(child.ChildText("Radius", "1"), 1.0f);
            sceneObject->CreatePrimitive<Disk>(normal, radius);
        }
    }
}

void LoadLights(Scene* scene, const XmlElement& lightsElement)
{
    for (const XmlElement& child : lightsElement.children)
    {
        if (child.name == "DirectionalLight")
        {
            const Vector3f direction = ParseVector3(child.ChildText("Direction", "0, -1, 0"));
            const Color radiance = ParseVector3(child.ChildText("Radiance", "1, 1, 1"));
            scene->CreateLight<DirectionalLight>(direction, radiance);
        }
        else if (child.name == "PointLight")
        {
            const Vector3f position = ParseVector3(child.ChildText("Position", "0, 0, 0"));
            const Color intensity = ParseVector3(child.ChildText("Intensity", "1, 1, 1"));
            const Vector3f attenuations = ParseVector3(child.ChildText("Attenuations", "1, 0, 0"));
            scene->CreateLight<PointLight>(position, intensity, attenuations);
        }
        else if (child.name == "SpotLight")
        {
            const Vector3f position = ParseVector3(child.ChildText("Position", "0, 0, 0"));
            const Vector3f direction = ParseVector3(child.ChildText("Direction", "0, -1, 0"));
            const Color intensity = ParseVector3(child.ChildText("Intensity", "1, 1, 1"));
            const float innerAngle = glm::radians(ParseFloat(child.ChildText("InnerAngle", "10"), 10.0f));
            const float outerAngle = glm::radians(ParseFloat(child.ChildText("OuterAngle", "60"), 60.0f));
            const Vector3f attenuations = ParseVector3(child.ChildText("Attenuations", "1, 0, 0"));
            scene->CreateLight<SpotLight>(position, direction, intensity, innerAngle, outerAngle, attenuations);
        }
    }
}

void LoadMaterial(SceneObject* sceneObject, const XmlElement& materialElement)
{
    if (const XmlElement* lambert = materialElement.Child("Lambert"))
    {
        const Color albedo = ParseVector3(lambert->ChildText("Albedo", "1, 1, 1"));
        sceneObject->CreateMaterial<LambertMaterial>(albedo);
    }
}
} // namespace

Scene* Scene::LoadFromXML(const std::string& filename)
{
    const std::string fileText = ReadFileText(filename);
    if (fileText.empty())
    {
        std::cerr << "Failed to read scene XML: " << filename << '\n';
        return nullptr;
    }

    const std::string xml = StripXmlComments(fileText);
    size_t offset = 0;
    XmlElement root;
    if (!ParseXmlElement(xml, offset, root) || root.name != "Scene")
    {
        std::cerr << "Invalid scene XML root in: " << filename << '\n';
        return nullptr;
    }

    Scene* scene = new Scene();

    if (const XmlElement* cameraElement = root.Child("Camera"))
    {
        const Vector3f position = ParseVector3(cameraElement->ChildText("Position", "0, 0, 0"));
        const Vector3f target = ParseVector3(cameraElement->ChildText("Target", "0, 0, 1"));
        const Vector3f up = ParseVector3(cameraElement->ChildText("Up", "0, 1, 0"));
        const float nearZ = ParseFloat(cameraElement->ChildText("NearZ", "0.1"), 0.1f);
        const float farZ = ParseFloat(cameraElement->ChildText("FarZ", "1000"), 1000.0f);
        const float fovDegrees = ParseFloat(cameraElement->ChildText("Fov", "60"), 60.0f);

        // XML 未提供分辨率时使用默认视口
        constexpr int defaultWidth = 800;
        constexpr int defaultHeight = 600;

        Camera camera;
        camera.Initialize(
            position,
            target,
            up,
            glm::radians(fovDegrees),
            nearZ,
            farZ,
            defaultWidth,
            defaultHeight);
        scene->SetCamera(camera);
    }

    if (const XmlElement* sceneObjectsElement = root.Child("SceneObjects"))
    {
        for (const XmlElement* objectElement : sceneObjectsElement->Children("SceneObject"))
        {
            Vector3f position(0.0f);
            Vector3f rotation(0.0f);
            float scale = 1.0f;

            if (const XmlElement* transform = objectElement->Child("Transform"))
            {
                position = ParseVector3(transform->ChildText("Position", "0, 0, 0"));
                rotation = ParseDegreesVector3(transform->ChildText("Rotation", "0, 0, 0"));
                scale = ParseFloat(transform->ChildText("Scale", "1"), 1.0f);
            }

            SceneObject* sceneObject = scene->CreateSceneObject(position, rotation, scale);
            if (const XmlElement* material = objectElement->Child("Material"))
            {
                LoadMaterial(sceneObject, *material);
            }
            if (const XmlElement* primitives = objectElement->Child("Primitives"))
            {
                LoadPrimitives(sceneObject, *primitives);
            }
        }
    }

    if (const XmlElement* lightsElement = root.Child("Lights"))
    {
        LoadLights(scene, *lightsElement);
    }

    return scene;
}

SceneObject* Scene::CreateSceneObject(const Vector3f& position, const Vector3f& euler, float scale)
{
    auto sceneObject = std::make_unique<SceneObject>(position, euler, scale);
    SceneObject* raw = sceneObject.get();
    mSceneObjects.push_back(std::move(sceneObject));
    return raw;
}

SceneObject* Scene::Intersect(Ray ray, Intersection& isect) const
{
    SceneObject* hitObject = nullptr;

    for (const auto& sceneObject : mSceneObjects)
    {
        Intersection candidate;
        // 必须尊重 mint/maxt：阴影射线用 mint 避免自相交导致整图发黑
        if (sceneObject->Intersect(ray, candidate)
            && candidate.t >= ray.mint
            && candidate.t < ray.maxt)
        {
            isect = candidate;
            isect.material = sceneObject->GetMaterial();
            ray.maxt = isect.t;
            hitObject = sceneObject.get();
        }
    }

    return hitObject;
}
