#pragma once 

#include <string>

#include "gl_base_element.h"

class texture : public gl_base_element 
{
public:
	texture() = default;
	texture(const std::string& filepath);
	~texture() override;

	void bind() override;

	void bind_slot(u32 slot = 0);

	void unbind() override;
	void delete_id() override;
	void set_data(const void* data, u32 width, u32 height, GLenum format);

	u32 get_width() const { return m_width; };
	u32 get_height() const { return m_height; };
	u32 get_channels() const { return m_n_channels; };

private:
	i32 m_width = 0;
	i32 m_height = 0;
	i32 m_n_channels = 0;
};
