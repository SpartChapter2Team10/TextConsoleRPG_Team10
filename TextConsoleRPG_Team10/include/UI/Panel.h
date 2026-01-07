#pragma once
#include <string>
#include <memory>
#include <vector>
#include <Windows.h>
#include "../Common/TextColor.h"  // ETextColor 사용을 위해 추가 (PrintManager 대신)

class ScreenBuffer;
class IContentRenderer;

// 패널 경계
struct PanelBounds {
    int X = 0;
    int Y = 0;
    int Width = 10;
    int Height = 10;

    PanelBounds() = default;
    PanelBounds(int x, int y, int w, int h) : X(x), Y(y), Width(w), Height(h) {}

    bool Contains(int x, int y) const {
        return x >= X && x < X + Width && y >= Y && y < Y + Height;
    }
};

// 렌더러 영역 (패널 내 독립적인 드로잉 구역)
struct RendererRegion {
    PanelBounds LocalBounds;  // 패널 내 상대 좌표
    std::unique_ptr<IContentRenderer> Renderer;
    
    RendererRegion(const PanelBounds& bounds, std::unique_ptr<IContentRenderer> renderer)
        : LocalBounds(bounds), Renderer(std::move(renderer)) {}
};

// 개별 UI 패널
class Panel
{
private:
    std::string _PanelID;
    PanelBounds _Bounds;
    
    // 렌더러 영역 목록 (각각 독립적인 영역을 가짐)
    std::vector<std::unique_ptr<RendererRegion>> _RendererRegions;
    
    bool _IsDirty = true;      // 재렌더링 필요 여부
    bool _HasBorder = false; // 테두리 표시 여부
    WORD _BorderColor = 7;     // 테두리 색상

public:
    Panel(const std::string& id, const PanelBounds& bounds);
    ~Panel();

    // ===== 기존 API (호환성 유지) =====
    
    // 콘텐츠 렌더러 설정
    // 주의: 이 메서드는 기존의 모든 렌더러를 제거하고 새로운 렌더러를 전체 영역에 추가합니다.
    void SetContentRenderer(std::unique_ptr<IContentRenderer> renderer);
    
    // 첫 번째 렌더러 가져오기 (기존 코드 호환)
    IContentRenderer* GetContentRenderer();

    // ===== 영역 분할 API =====
    
    // 렌더러 추가 (특정 영역 지정)
    // localX, localY: 패널 내 상대 좌표 (0,0 = 패널 왼쪽 상단)
    // width, height: 렌더러가 사용할 영역 크기
    // 주의: 영역이 겹치면 나중에 추가된 렌더러가 위에 표시됩니다.
    void AddRenderer(int localX, int localY, int width, int height, 
          std::unique_ptr<IContentRenderer> renderer);
    
    // 모든 렌더러 제거
    void ClearRenderers();
    
    // 특정 인덱스의 렌더러 제거
    void RemoveRenderer(size_t index);

    // 모든 렌더러 업데이트 (애니메이션, 타이핑 효과 등)
    void Update(float deltaTime);

    // 패널을 화면 버퍼에 렌더링
    void RenderToBuffer(ScreenBuffer& buffer);

 // 강제 재렌더링
void Redraw() { _IsDirty = true; }

    // 테두리 설정 (오버로드 추가)
    void SetBorder(bool enable, WORD color = 7);
    void SetBorder(bool enable, ETextColor color) { SetBorder(enable, static_cast<WORD>(color)); }

    // Dirty 플래그 관리
    inline void SetDirty() { _IsDirty = true; }
    inline bool IsDirty() const { return _IsDirty; }
inline void ClearDirty() { _IsDirty = false; }

    // Getter
    inline const std::string& GetID() const { return _PanelID; }
    inline const PanelBounds& GetBounds() const { return _Bounds; }
    inline PanelBounds& GetBounds() { return _Bounds; }
};
