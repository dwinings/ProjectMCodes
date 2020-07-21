//
// Created by johno on 7/18/2020.
//

#ifndef PROJECTMCODES_DRAW_H
#define PROJECTMCODES_DRAW_H

#include "Brawl/gfCameraManager.h"
#include "Wii/GX.h"
#include "Brawl/gfDraw.h"

void start2DDraw();
void startNormalDraw();
void setupDrawPrimitives();

void drawTriangleFan(GXColor color, Position3D* vertices, u32 numVertices);
void draw2DRectangle(GXColor color, float top, float bottom, float left, float right, float zPos = 0);
void drawPrimitive(GXColor color, Position3D* vertices, u32 numVertices);


#endif //PROJECTMCODES_DRAW_H
