#pragma once

struct DefineWidget
{
    DefineWidget(const char* name, void (*func)());
};

#ifndef _CONCAT
#define _CONCAT(a, b)       _CONCAT_IMPL(a, b)
#define _CONCAT_IMPL(a, b)  a ## b
#endif

#ifndef _SYMBOL
#define _SYMBOL(name)       _CONCAT(name, __LINE__)
#endif

#define DEFINE_WIDGET(name)                                             \
    static void _SYMBOL(WIDGET_FUNC)();                                 \
    static DefineWidget _SYMBOL(WIDGET)(name, _SYMBOL(WIDGET_FUNC));    \
    static void _SYMBOL(WIDGET_FUNC)()