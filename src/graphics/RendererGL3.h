// Copyright © 2008-2014 Pioneer Developers. See AUTHORS.txt for details
// Copyright © 2013-14 Meteoric Games Ltd
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef _RENDERER_GL3_H
#define _RENDERER_GL3_H
/*
 * OpenGL 3.1/3.2 renderer (3.1/3.2, 1.4/1.5 GLSL)
 *  - no fixed function support (shaders for everything)
 *  The plan is: make this more like GL3/ES2
 *  - try to stick to bufferobjects
 *  - use glvertexattribpointer instead of glvertexpointer etc
 *  - get rid of built-in glMaterial, glMatrix use
 */
#include "Renderer.h"
#include <stack>
#include <unordered_map>

namespace Graphics {

class Texture;
struct Settings;
namespace GL3 { 
	class Effect;
	class Program;
	class RenderState; 
	class RenderTarget; 
	class Material;
	class MultiMaterial;
	class LitMultiMaterial;
	class VertexBuffer;
}

class RendererGL3 : public Renderer
{
public:
	RendererGL3(WindowSDL *window, const Graphics::Settings &vs);
	virtual ~RendererGL3();

	virtual const char* GetName() const { return "GL3 renderer"; }
	virtual bool GetNearFarRange(float &near, float &far) const;

	virtual bool BeginFrame();
	virtual bool BeginPostProcessing(RenderTarget* rt_device);
	virtual bool PostProcessFrame(PostProcess* postprocess);
	virtual bool EndPostProcessing();
	virtual bool EndFrame();
	virtual bool SwapBuffers();

	virtual bool SetRenderState(RenderState*) override;
	virtual bool SetRenderTarget(RenderTarget*) override;

	// Warning: active render target Could be null! (indicating main framebuffer)
	virtual RenderTarget* GetActiveRenderTarget() const { return reinterpret_cast<Graphics::RenderTarget*>(m_activeRenderTarget); }

	virtual bool ClearScreen();
	virtual bool ClearDepthBuffer();
	virtual bool SetClearColor(const Color &c);

	virtual bool SetViewport(int x, int y, int width, int height);

	virtual bool SetTransform(const matrix4x4d &m);
	virtual bool SetTransform(const matrix4x4f &m);
	virtual bool SetPerspectiveProjection(float fov, float aspect, float near, float far);
	virtual bool SetOrthographicProjection(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
	virtual bool SetProjection(const matrix4x4f &m);

	virtual bool SetWireFrameMode(bool enabled);

	virtual bool SetLights(int numlights, const Light *l);
	virtual bool SetAmbientColor(const Color &c);

	virtual bool SetScissor(bool enabled, const vector2f &pos = vector2f(0.0f), const vector2f &size = vector2f(0.0f));

	virtual bool BeginDrawVB(VertexBuffer* vb, Material* mat = nullptr) override;
	virtual bool EndDrawVB(VertexBuffer* vb, Material* mat = nullptr) override;

	virtual bool DrawLines(int vertCount, const vector3f *vertices, const Color *colors, RenderState*, LineType type=LINE_SINGLE) override;
	virtual bool DrawLines(int vertCount, const vector3f *vertices, const Color &color, RenderState*, LineType type=LINE_SINGLE) override;
	virtual bool DrawLinesBuffer(int vertCount, VertexBuffer *vb, RenderState*, LineType type = LINE_SINGLE) override;

	virtual bool DrawLines(int vertCount, VertexArray *va, const Color *colors, RenderState*, LineType type = LINE_SINGLE) override;
	virtual bool DrawLines(int vertCount, VertexArray *va, const Color &color, RenderState*, LineType type = LINE_SINGLE) override;
	virtual bool DrawLines2D(int vertCount, const vector2f *vertices, const Color &color, RenderState*, LineType type=LINE_SINGLE) override;
	virtual bool DrawPoints(int count, const vector3f *points, const Color *colors, RenderState*, float pointSize=1.f) override;
	virtual bool DrawTriangles(VertexArray *vertices, RenderState *state, Material *material, PrimitiveType type=TRIANGLES) override;
	virtual bool DrawTriangles(int vertCount, VertexArray *vertices, RenderState *state, Material *material, PrimitiveType type=TRIANGLES);
	virtual bool DrawPointSprites(int count, const vector3f *positions, RenderState *rs, Material *material, float size) override;
	virtual bool DrawBuffer(VertexBuffer*, RenderState*, Material*, PrimitiveType) override;
	virtual bool DrawBufferIndexed(VertexBuffer*, IndexBuffer*, RenderState*, Material*, PrimitiveType, unsigned start_index = 0, unsigned index_count = 0) override;
	virtual bool DrawFullscreenQuad(Material* mat, RenderState* state = nullptr, bool clear_rt = true) override;
	virtual bool DrawFullscreenQuad(Texture* texture, RenderState* state = nullptr, bool clear_rt = true) override;
	virtual bool DrawFullscreenQuad(RenderState *state, bool clear_rt = true) override;

	virtual Material *CreateMaterial(const MaterialDescriptor &descriptor) override;
	virtual Texture *CreateTexture(const TextureDescriptor &descriptor) override;
	virtual RenderState *CreateRenderState(const RenderStateDesc &) override;
	virtual RenderTarget *CreateRenderTarget(const RenderTargetDesc &) override;
	virtual VertexBuffer *CreateVertexBuffer(const VertexBufferDesc&) override;
	virtual IndexBuffer *CreateIndexBuffer(Uint32 size, BufferUsage) override;

	virtual void SetEffect(GL3::Effect* effect) override;

	virtual bool ReloadShaders();

	virtual bool PrintDebugInfo(std::ostream &out);

	virtual const matrix4x4f& GetCurrentModelView() const { return m_modelViewStack.top(); }
	virtual const matrix4x4f& GetCurrentProjection() const { return m_projectionStack.top(); }

	virtual void SetMatrixMode(MatrixMode mm);
	virtual void PushMatrix();
	virtual void PopMatrix();
	virtual void LoadIdentity();
	virtual void LoadMatrix(const matrix4x4f &m);
	virtual void Translate( const float x, const float y, const float z );
	virtual void Scale( const float x, const float y, const float z );

	virtual float GetInvLogZfarPlus1() const override { return m_invLogZfarPlus1; }

protected:

	//figure out states from a vertex array and enable them
	//also sets vertex pointers
	void EnableClientStates(const VertexArray*);
	void EnableClientStates(const VertexBuffer*);
	//disable previously enabled
	virtual void DisableClientStates();
	int m_numLights;

    unsigned m_defaultVAO;
	std::vector<GLenum> m_clientStates;
	float m_minZNear;
	float m_maxZFar;
	bool m_useCompressedTextures;

	matrix4x4f& GetCurrentTransform() { return m_currentTransform; }
	matrix4x4f m_currentTransform;
	GL3::Program* GetOrCreateProgram(GL3::Material*);

	friend class GL3::Material;
	friend class GL3::MultiMaterial;
	friend class GL3::LitMultiMaterial;

	std::unordered_map<Uint32, GL3::RenderState*> m_renderStates;
	float m_invLogZfarPlus1;
	GL3::RenderTarget *m_activeRenderTarget;
	RenderState *m_activeRenderState;
	GL3::Effect *m_activeEffect;

	MatrixMode m_matrixMode;
	std::stack<matrix4x4f> m_modelViewStack;
	std::stack<matrix4x4f> m_projectionStack;

	std::unique_ptr<GL3::VertexBuffer> m_screenQuadVB;
	RenderState* m_screenQuadRS;

	std::unique_ptr<VertexArray> m_linesVA;
	std::unique_ptr<VertexArray> m_linesDiffuseVA;
	std::unique_ptr<VertexArray> m_pointsVA;

};

}

#endif
