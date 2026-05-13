/*  models.cpp  –  OpenGL 3D model implementations
 *
 *  Floor model  : subdivided XZ plane, tiled UVs, flat +Y normals
 *  Horse model  : ~20 axis-aligned boxes assembled into a horse silhouette,
 *                 each box face has its own outward normal and [0,1]² UVs.
 *
 *  Both models are uploaded once (GL_STATIC_DRAW) into separate VAO/VBO/EBO
 *  pairs so they can be drawn independently with different shader uniforms.
 */

#include "models.h"
#include <vector>
#include <cstddef>   // offsetof

/* ──────────────────────────────────────────────────────────────────────────
 *  Internal vertex layout
 * ────────────────────────────────────────────────────────────────────────── */

struct Vertex {
    float pos[3];   // XYZ position
    float nor[3];   // XYZ normal (unit vector)
    float uv[2];    // UV texture coordinate
};

/* ──────────────────────────────────────────────────────────────────────────
 *  Box helper
 *
 *  Appends the 6 faces of an axis-aligned box to `verts` and `idxs`.
 *
 *  cx,cy,cz  – centre of the box in world space
 *  hx,hy,hz  – half-extents along each axis
 *
 *  Each face gets 4 unique vertices (for flat per-face normals) and
 *  2 triangles (6 indices). Winding is counter-clockwise when viewed
 *  from outside the box (OpenGL default front-face = CCW).
 * ────────────────────────────────────────────────────────────────────────── */

static void addBox(std::vector<Vertex>&        verts,
                   std::vector<unsigned int>&  idxs,
                   float cx, float cy, float cz,
                   float hx, float hy, float hz)
{
    /* UV corners shared by every face: TL, TR, BR, BL */
    static const float UV[4][2] = { {0,1},{1,1},{1,0},{0,0} };

    /* Six faces: normal + 4 corner offsets (relative to box centre) */
    struct Face {
        float nx, ny, nz;
        float c[4][3];   // 4 corner offsets
    };

    const Face faces[6] = {
        /* +X */ { 1, 0, 0, {{ hx, hy, hz}, { hx,-hy, hz}, { hx,-hy,-hz}, { hx, hy,-hz}} },
        /* -X */ {-1, 0, 0, {{-hx, hy,-hz}, {-hx,-hy,-hz}, {-hx,-hy, hz}, {-hx, hy, hz}} },
        /* +Y */ { 0, 1, 0, {{-hx, hy,-hz}, { hx, hy,-hz}, { hx, hy, hz}, {-hx, hy, hz}} },
        /* -Y */ { 0,-1, 0, {{-hx,-hy, hz}, { hx,-hy, hz}, { hx,-hy,-hz}, {-hx,-hy,-hz}} },
        /* +Z */ { 0, 0, 1, {{-hx, hy, hz}, { hx, hy, hz}, { hx,-hy, hz}, {-hx,-hy, hz}} },
        /* -Z */ { 0, 0,-1, {{ hx, hy,-hz}, {-hx, hy,-hz}, {-hx,-hy,-hz}, { hx,-hy,-hz}} },
    };

    for (const Face& f : faces) {
        unsigned int base = static_cast<unsigned int>(verts.size());
        for (int v = 0; v < 4; v++) {
            Vertex vtx;
            vtx.pos[0] = cx + f.c[v][0];
            vtx.pos[1] = cy + f.c[v][1];
            vtx.pos[2] = cz + f.c[v][2];
            vtx.nor[0] = f.nx;
            vtx.nor[1] = f.ny;
            vtx.nor[2] = f.nz;
            vtx.uv[0]  = UV[v][0];
            vtx.uv[1]  = UV[v][1];
            verts.push_back(vtx);
        }
        /* Two CCW triangles: 0-1-2 and 0-2-3 */
        idxs.push_back(base + 0); idxs.push_back(base + 1); idxs.push_back(base + 2);
        idxs.push_back(base + 0); idxs.push_back(base + 2); idxs.push_back(base + 3);
    }
}

/* ──────────────────────────────────────────────────────────────────────────
 *  GPU upload helper
 *
 *  Creates and configures VAO, VBO, EBO for one model.
 * ────────────────────────────────────────────────────────────────────────── */

static Model uploadToGPU(const std::vector<Vertex>&       verts,
                          const std::vector<unsigned int>& idxs)
{
    Model m;
    m.indexCount = static_cast<GLsizei>(idxs.size());
    m.drawMode   = GL_TRIANGLES;

    glGenVertexArrays(1, &m.VAO);
    glGenBuffers(1, &m.VBO);
    glGenBuffers(1, &m.EBO);

    glBindVertexArray(m.VAO);

    /* Upload vertex data */
    glBindBuffer(GL_ARRAY_BUFFER, m.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(verts.size() * sizeof(Vertex)),
                 verts.data(),
                 GL_STATIC_DRAW);

    /* Upload index data – binding an EBO while a VAO is active stores it
     * inside that VAO, so drawModel only needs to bind the VAO. */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(idxs.size() * sizeof(unsigned int)),
                 idxs.data(),
                 GL_STATIC_DRAW);

    /* Attribute 0 – position (vec3) */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, pos)));
    glEnableVertexAttribArray(0);

    /* Attribute 1 – normal (vec3) */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, nor)));
    glEnableVertexAttribArray(1);

    /* Attribute 2 – texcoord (vec2) */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, uv)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);   /* unbind – never unbind EBO before the VAO */
    return m;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  PUBLIC API
 * ══════════════════════════════════════════════════════════════════════════ */

/* ──────────────────────────────────────────────────────────────────────────
 *  Floor model
 *
 *  A flat 20 × 20 unit plane (±10 on X and Z) lying in the XZ plane at Y=0.
 *  Subdivided into a 4 × 4 grid of quads so lighting looks smooth.
 *  Normals all point straight up (+Y).
 *  UVs are tiled: every cell spans [0,2.5] so a 1-unit texture tiles nicely.
 * ────────────────────────────────────────────────────────────────────────── */

Model createFloorModel()
{
    std::vector<Vertex>       verts;
    std::vector<unsigned int> idxs;

    const int   GRID  = 4;       /* number of cells per side */
    const float HALF  = 10.0f;   /* half-extent of the plane */
    const float STEP  = (2.0f * HALF) / GRID;
    const float UV_SCALE = 2.5f; /* UV units per grid cell (controls tiling) */

    /* Build a (GRID+1) × (GRID+1) grid of vertices */
    for (int row = 0; row <= GRID; row++) {
        for (int col = 0; col <= GRID; col++) {
            Vertex v;
            v.pos[0] = -HALF + col * STEP;
            v.pos[1] =  0.0f;
            v.pos[2] = -HALF + row * STEP;
            v.nor[0] = 0.0f; v.nor[1] = 1.0f; v.nor[2] = 0.0f;
            v.uv[0]  = col * UV_SCALE;
            v.uv[1]  = row * UV_SCALE;
            verts.push_back(v);
        }
    }

    /* Build two CCW triangles per quad cell */
    const int W = GRID + 1;
    for (int row = 0; row < GRID; row++) {
        for (int col = 0; col < GRID; col++) {
            unsigned int tl = row * W + col;
            unsigned int tr = tl + 1;
            unsigned int bl = tl + W;
            unsigned int br = bl + 1;
            idxs.push_back(tl); idxs.push_back(bl); idxs.push_back(tr);
            idxs.push_back(tr); idxs.push_back(bl); idxs.push_back(br);
        }
    }

    return uploadToGPU(verts, idxs);
}

/* ──────────────────────────────────────────────────────────────────────────
 *  Trojan horse model  (simplified)
 *
 *  7 non-overlapping axis-aligned boxes → 84 triangles (7 × 6 faces × 2).
 *  Each part is sized and placed so the silhouette reads as a horse from
 *  every cardinal direction: front (+Z), back (-Z), left (-X), right (+X),
 *  top (+Y).
 *
 *  Coordinate conventions:
 *    Y up, +Z = horse faces forward
 *    Hooves touch Y = 0, head crown reaches Y ≈ 1.0
 *
 *  Bounding box (approx): X ±0.34, Y 0–1.0, Z ±0.60
 *
 *  Part layout           cx      cy      cz      hx     hy     hz
 *  ─────────────────────────────────────────────────────────────
 *  Body                  0      0.38     0      0.26   0.18   0.52
 *  Front-left leg      -0.16   0.10    0.36    0.08   0.10   0.08
 *  Front-right leg     +0.16   0.10    0.36    0.08   0.10   0.08
 *  Back-left leg       -0.16   0.10   -0.36    0.08   0.10   0.08
 *  Back-right leg      +0.16   0.10   -0.36    0.08   0.10   0.08
 *  Neck                  0      0.68    0.50    0.10   0.12   0.10
 *  Head                  0      0.88    0.58    0.10   0.10   0.22
 * ────────────────────────────────────────────────────────────────────────── */

Model createHorseModel()
{
    std::vector<Vertex>       verts;
    std::vector<unsigned int> idxs;

    /*  addBox(verts, idxs,  cx,     cy,     cz,     hx,    hy,    hz)  */

    /* ── BODY ─────────────────────────────────────────────────────────
     *  Wide enough (hx=0.26) to frame both leg pairs from front/back.
     *  Bottom of body (y=0.20) sits flush on top of the legs.        */
    addBox(verts, idxs,   0.00f,  0.38f,  0.00f,  0.26f, 0.18f, 0.52f);

    /* ── LEGS ─────────────────────────────────────────────────────────
     *  Legs span Y 0–0.20 (hy=0.10, cy=0.10).
     *  X offset ±0.16 gives clear separation from both front and back.
     *  Z offset ±0.36 aligns with the front/back face of the body.   */
    addBox(verts, idxs,  -0.16f,  0.10f,  0.36f,  0.08f, 0.10f, 0.08f); /* front-left  */
    addBox(verts, idxs,   0.16f,  0.10f,  0.36f,  0.08f, 0.10f, 0.08f); /* front-right */
    addBox(verts, idxs,  -0.16f,  0.10f, -0.36f,  0.08f, 0.10f, 0.08f); /* back-left   */
    addBox(verts, idxs,   0.16f,  0.10f, -0.36f,  0.08f, 0.10f, 0.08f); /* back-right  */

    /* ── NECK ─────────────────────────────────────────────────────────
     *  Sits on top of the body's front edge (cy-hy = 0.56 = body top).
     *  Visible as a distinct vertical block from all four sides.      */
    addBox(verts, idxs,   0.00f,  0.68f,  0.50f,  0.10f, 0.12f, 0.10f);

    /* ── HEAD ─────────────────────────────────────────────────────────
     *  Long along Z (hz=0.22) for a pronounced muzzle from the side.
     *  Wide along X (hx=0.10) so the face is readable from the front.
     *  Bottom of head (y=0.78) meets top of neck (y=0.80) closely.  */
    addBox(verts, idxs,   0.00f,  0.88f,  0.58f,  0.10f, 0.10f, 0.22f);

    return uploadToGPU(verts, idxs);
}

/* ──────────────────────────────────────────────────────────────────────────
 *  Draw / Destroy
 * ────────────────────────────────────────────────────────────────────────── */

void drawModel(const Model& model)
{
    glBindVertexArray(model.VAO);
    glDrawElements(model.drawMode,
                   model.indexCount,
                   GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}

void destroyModel(Model& model)
{
    glDeleteBuffers(1,      &model.EBO);
    glDeleteBuffers(1,      &model.VBO);
    glDeleteVertexArrays(1, &model.VAO);
    model.VAO = model.VBO = model.EBO = 0;
    model.indexCount = 0;
}
