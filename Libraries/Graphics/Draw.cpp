//
// Created by johno on 7/18/2020.
//

#include "Draw.h"

void start2DDraw() {
    _gfDrawSetupCoord2D();
}

void setupDrawPrimitives() {
    //assumes you never want your primitives to be culled
    _GXSetCullMode(GXCullMode::GX_CULL_NONE);
    _gfDrawSetVtxPosColorPrimEnvironment();
    _GXSetZMode(true, GXCompare::GX_EQUAL, false);
    _GXSetAlphaCompare(GXCompare::GX_ALWAYS, 0, 1, GXCompare::GX_ALWAYS, 0);
}

void startNormalDraw() {
    auto manager = _getManager_gfCameraManager();
    _setGX_gfCamera(manager->cameras);
}

void drawTriangleFan(GXColor color, Position3D* vertices, u32 numVertices) {
    _GXBegin(GXPrimitive::GX_TRIANGLEFAN, 1, numVertices);

    drawPrimitive(color, vertices, numVertices);
}

void draw2DRectangle(GXColor color, float top, float bottom, float left, float right, float zPos) {
    _GXBegin(GXPrimitive::GX_QUADS, 1, 4);

    WG_PIPE->F32 = left;
    WG_PIPE->F32 = top;
    WG_PIPE->F32 = zPos;
    WG_PIPE->U32 = color.value;

    WG_PIPE->F32 = right;
    WG_PIPE->F32 = top;
    WG_PIPE->F32 = zPos;
    WG_PIPE->U32 = color.value;

    WG_PIPE->F32 = right;
    WG_PIPE->F32 = bottom;
    WG_PIPE->F32 = zPos;
    WG_PIPE->U32 = color.value;

    WG_PIPE->F32 = left;
    WG_PIPE->F32 = bottom;
    WG_PIPE->F32 = zPos;
    WG_PIPE->U32 = color.value;
}


/*void drawQuads(GXColor color, Position3D* vertices, u32 numVertices) {
    _GXBegin(GXPrimitive::GX_QUADS, 1, numVertices);

    drawPrimitive(color, vertices, numVertices);
}*/


void drawPrimitive(GXColor color, Position3D* vertices, u32 numVertices) {
    for(int i = 0; i < numVertices; i++) {
        WG_PIPE->F32 = vertices[i].x;
        WG_PIPE->F32 = vertices[i].y;
        WG_PIPE->F32 = vertices[i].z;
        WG_PIPE->U32 = color.value;
    }
}