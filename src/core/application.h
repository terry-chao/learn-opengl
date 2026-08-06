#pragma once

#include "window.h"

#include <memory>

class application
{
public:
    application();
    virtual ~application();

    application(const application&) = delete;
    application& operator=(const application&) = delete;

    void run();

    window& get_window() { return *m_window; }
    static application& get() { return *s_instance; }

protected:
    virtual void on_init() {}
    virtual void on_update(float delta_time) { (void)delta_time; }
    virtual void on_render() {}
    virtual void on_shutdown() {}

private:
    std::unique_ptr<window> m_window;
    bool m_running = true;

    static application* s_instance;
};
