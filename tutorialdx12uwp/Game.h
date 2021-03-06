//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "Mesh.h"
#include "HelperFunctions.h"
#include "DDSTextureLoader.h"
#include "Controller.h"


#pragma comment (lib, "Windowscodecs.lib")

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game
{
public:

    Game() noexcept ;

    // Initialization and management
    void Initialize(::IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);
    void InitializeObjects(const std::vector<int>& numberOfInstances, const std::vector<int>& matInd, const float r, const float minDistance, const float maxDistance);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
    void ValidateDevice();

    // Properties
    void GetDefaultSize( int& width, int& height ) const;
    void LoadMeshes();
    void LoadSprites();
    void RenderUI();
    void SetDPI(float x, float y);

private:

    //DPI for Direct2 rendering
    struct dpi {
        float xdpi;
        float ydpi;
    } m_dpi;
    
    size_t m_NumberOfMeshes;

    const size_t c_NumberOfObjects = 5;
    const size_t c_NumberOfInstancesPerObject = 3;

	size_t instancedObj;

    // Per object particular information
    struct ObjectData {
        bool                                                isInstanced;
        XMFLOAT4X4											matrixWorld;
        UINT                                                matind;
		bool                                                hide;
		int													score;
     };
   
    // One shape each time
    std::vector<std::vector<ObjectData>> m_objects; // Each element is per shape information. Each per shape information is per instance data.
    
    std::vector<std::shared_ptr<Mesh>>					m_meshes; // One mesh per shape


    XMFLOAT4X4											m_view;
    XMFLOAT4X4											m_projection;


    void CreateMainInputFlowResources(const std::vector<std::shared_ptr<Mesh>>& mesh);
    

    // Vertex and index related stuff
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_vBufferDefault; // Buffer para v?rtices
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_vBufferUpload; // Buffer para v?rtices
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_iBufferDefault; // Buffer para indices
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_iBufferUpload; // Buffer para indices
    D3D12_VERTEX_BUFFER_VIEW				m_vBufferView;
    D3D12_INDEX_BUFFER_VIEW				m_iBufferView;
    
    // Textures related stuff

    
    
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>  m_textureDefault;
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>  m_textureUpload;


    // Constants resources and descriptores related stuff

    // Constants are "frame resources". We use as many frame resources as swap chain buffers.
    static const UINT                                   c_swapBufferCount = 3;

    // Pass constants
    struct vConstants {

        DirectX::XMFLOAT4X4 PassTransform = { 1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0 };


    };

    // Instance object constants
    struct vInstance {

        DirectX::XMFLOAT4X4 Transform = { 1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0 };
        DirectX::XMFLOAT4X4 NormalTransform = { 1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0 };
        UINT MaterialIndex;
        UINT Pad0;
        UINT Pad1;
        UINT Pad2;
		bool Hide;
		

    };

    // Data:
    vConstants                                                       m_vConstants[c_swapBufferCount];
    std::vector<std::vector<vInstance>>                              m_vInstances[c_swapBufferCount]; // Vector of instances per object.

    // One pass constant buffer per frame resource.
    Microsoft::WRL::ComPtr<ID3D12Resource>				m_vConstantBuffer[c_swapBufferCount]; // Buffer de constantes
    
    // One instance constant buffer per object and frame resource.
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>				m_vInstanceBuffer[c_swapBufferCount]; // Buffer de constantes
   
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_cDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_sDescriptorHeap; // Descriptor HEap de Samplers
    UINT m_cDescriptorSize;


    Microsoft::WRL::ComPtr<ID3D12RootSignature>         m_rootSignature;




    

    void LoadPrecompiledShaders();

    Microsoft::WRL::ComPtr<ID3DBlob>					m_vsByteCode;
    Microsoft::WRL::ComPtr<ID3DBlob>					m_psByteCode;
    D3D12_SHADER_BYTECODE								m_vs;
    D3D12_SHADER_BYTECODE								m_ps;

    void PSO();

    std::vector<D3D12_INPUT_ELEMENT_DESC>				m_inputLayout;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC		m_psoDescriptor;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>		m_pso;

    // Updates
    void Update(DX::StepTimer const& timer);
    XMMATRIX UpdateView();

    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void WaitForGpu();
    void MoveToNextFrame();
    void GetAdapter(IDXGIAdapter1** ppAdapter);

    void OnDeviceLost();

    

    // Application state
    IUnknown*                                           m_window;
    int                                                 m_outputWidth;
    int                                                 m_outputHeight;
    DXGI_MODE_ROTATION                                  m_outputRotation;

    // Direct3D12 Objects
    D3D_FEATURE_LEVEL                                   m_featureLevel;
    
    UINT                                                m_backBufferIndex;
    UINT                                                m_rtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
    Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[c_swapBufferCount];

    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
    Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
    UINT64                                              m_fenceValues[c_swapBufferCount];

    Microsoft::WRL::Wrappers::Event                     m_fenceEvent;


    // D3D1211ON12 objects
    Microsoft::WRL::ComPtr<ID3D11Device>                m_d3d11Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
    Microsoft::WRL::ComPtr<ID3D11On12Device1> m_d3d11On12Device;
    Microsoft::WRL::ComPtr<IDWriteFactory> m_dWriteFactory;
    Microsoft::WRL::ComPtr<ID2D1Factory3> m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1Device2> m_d2dDevice;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext2> m_d2dDeviceContext;

   

    // Rendering resources
    Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;

    // Rendering resources for D3D1211ON12 and Direct2D
    Microsoft::WRL::ComPtr<ID3D11Resource> m_wrappedBackBuffers[c_swapBufferCount];
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_d2dRenderTargets[c_swapBufferCount];

    // App Resouces

    // App Resources for Direct2D and DirectWrite
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_textBrush;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;

    // Sprites related stuff
    // Use the Windows Imaging Component.
    Microsoft::WRL::ComPtr<IWICImagingFactory2> m_WICFactory;
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> m_bitmapDecoder;
    Microsoft::WRL::ComPtr<IWICFormatConverter> m_formatConverter;
    // Pointer
    Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_crossBitmap;
    
    
    // Game state
    DX::StepTimer                                       m_timer;

    //Input
    //Input
    std::shared_ptr<Controller> m_controller;
    XMVECTOR m_Position;
    XMVECTOR m_LookDirection;

    // Game state
    UINT m_score;
};

namespace GameStatics {

    const UINT MaxNumberOfMeshes = 10;

    enum class ShapeName { SHAPE1=0 , SHAPE2=1, SHAPE3=2, SHAPE4=3, SHAPE5=4 };
    static std::map<ShapeName,std::string>  ObjFileNames = {
        {ShapeName::SHAPE1,"Assets/mesh1.obj"},
        {ShapeName::SHAPE2,"Assets/mesh2.obj"},
        {ShapeName::SHAPE3,"Assets/mesh3.obj"},
        {ShapeName::SHAPE4,"Assets/mesh4.obj"},
        {ShapeName::SHAPE5,"Assets/mesh5.obj"}
    };
    
    enum class TexName { TEX1=0,TEX2=1,TEX3=2,TEX4=3,TEX5=4};
    static std::map<TexName, std::wstring> TexFileNames = {
        {TexName::TEX1,L"Assets/tex1.dds"},
        {TexName::TEX2,L"Assets/tex2.dds"},
        {TexName::TEX3,L"Assets/tex3.dds"},
        {TexName::TEX4,L"Assets/tex4.dds"},
        {TexName::TEX5,L"Assets/tex5.dds"}
    };

    enum class BitmapName {BMP1=0,BMP2=1,BMP3=2,BMP4=3,BMP5=4 };
    static std::map<BitmapName, std::wstring> BitmapFileNames = {
        {BitmapName::BMP1,L"Assets/sprite1.png"},
        {BitmapName::BMP2,L"Assets/sprite2.png"},
        {BitmapName::BMP3,L"Assets/sprite3.png"},
        {BitmapName::BMP4,L"Assets/sprite4.png"},
        {BitmapName::BMP5,L"Assets/sprite5.png"}
    };

}