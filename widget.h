#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>
#include <dwmapi.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_6.h>
#include <windows.h>

#include <inspectable.h>
#include <winrt/base.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <windows.graphics.directx.direct3d11.interop.h>
#include <Windows.Graphics.Capture.Interop.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Graphics.Capture.h>




using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;
using namespace winrt::Windows::Graphics;
using namespace winrt::Windows::Graphics::DirectX;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;
using namespace winrt::Windows::Graphics::Capture;


class GraphicsCapture
{
public:
    using Callback = std::function<void(ID3D11Texture2D*, int w, int h)>;

    GraphicsCapture(const com_ptr<ID3D11Device>& o) {m_device = o;}

    bool start(HWND hwnd, bool free_threaded, const Callback& callback);
    void stop();

private:
    void onFrameArrived(
        winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender,
        winrt::Windows::Foundation::IInspectable const& args);

private:
    Callback m_callback;
    com_ptr<ID3D11Device> m_device;

    IDirect3DDevice m_device_rt{ nullptr };
    Direct3D11CaptureFramePool m_frame_pool{ nullptr };
    GraphicsCaptureItem m_capture_item{ nullptr };
    GraphicsCaptureSession m_capture_session{ nullptr };
    Direct3D11CaptureFramePool::FrameArrived_revoker m_frame_arrived;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

   public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

   public:
    QPaintEngine *paintEngine() const { return nullptr; }

    float bgcolor[4] = {1, 0.5, 0.5, 1};   //我自定义清空背景颜色的颜色值
    ID3D11Device *m_d3dDevice;             //创建D3D主要接口
    ID3D11DeviceContext *m_d3dDevContext;  //创建D3D绘制上下文
    IDXGISwapChain *m_swapChain;           //交换链
    ID3D11Texture2D *m_depthStencilBuffer;  //深度和模板缓冲区的Buffer缓存，可以理解为一张窗口图片
    ID3D11DepthStencilView *m_depthStencilView;  //深度模板缓冲区窗口描述
    ID3D11RenderTargetView *m_renderTargetView;  //绘制窗口描述
    ID3D11RasterizerState *m_rasterizeState;     //状态

    D3D11_VIEWPORT viewport;  //我们所看到的视口
   public:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void InitD3D();
    void ResizeD3D();
    void RenderInit();
    void RenderUpdate();

   private:
    Ui::Widget *ui;
};
#endif  // WIDGET_H
