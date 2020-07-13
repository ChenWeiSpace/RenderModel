#ifndef RenderContent_h__
#define RenderContent_h__
#include <memory>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
typedef std::shared_ptr<class RenderCamera> RenderCameraPtr;

class RenderContent : public std::enable_shared_from_this<RenderContent>
{
public:
	RenderContent();
	~RenderContent();
	void initDevice(int wHid, int w,int h);

	void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed);
private:
	void buildRenderCube();
	void renderCube(float t);
private:
	int m_width{0};
	int m_height{0};

	ID3D11Device*           g_pd3dDevice{ NULL };
	ID3D11DeviceContext*    g_pImmediateContext{ NULL };
	IDXGISwapChain*         g_pSwapChain{ NULL };

	ID3D11RenderTargetView* g_pRenderTargetView{ NULL };
	ID3D11DepthStencilView* g_pDepthStencilView = { NULL };
	RenderCameraPtr m_camera;

	//copy texture
	//ID3D11ShaderResourceView* m_copyResView{NULL};
	//cube
	ID3D11VertexShader*     g_pVertexShader{ NULL };
	ID3D11InputLayout*      g_pVertexLayout{ NULL };
	ID3D11PixelShader*      g_pPixelShader{ NULL };
	ID3D11Buffer*           g_pConstantBuffer{ NULL };
	ID3D11Buffer*           g_pVertexBuffer{ NULL };
	ID3D11Buffer*           g_pIndexBuffer{ NULL };
};
typedef std::shared_ptr<RenderContent> RenderContentPtr;
#endif // RenderContent_h__
