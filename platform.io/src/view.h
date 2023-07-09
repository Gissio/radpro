/*
 * Rad Pro
 * User interface view manager
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef VIEW_H
#define VIEW_H

#include <stdint.h>

#include "keyboard.h"

struct View;

typedef void OnViewDraw(const struct View *view);
typedef void OnViewKey(const struct View *view, KeyEvent keyEvent);

struct View
{
    OnViewDraw *onDraw;
    OnViewKey *onKey;
    const void *userdata;
};

void updateView(void);

void setView(const struct View *view);
const struct View *getView(void);
void refreshView(void);

#endif
