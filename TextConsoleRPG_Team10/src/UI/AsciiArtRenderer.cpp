#include "../../include/UI/AsciiArtRenderer.h"
#include "../../include/UI/ScreenBuffer.h"
#include "../../include/UI/Panel.h"
#include "../../include/Manager/DataManager.h"
#include "../../include/Manager/PrintManager.h"
#include <sstream>
#include <algorithm>

bool AsciiArtRenderer::LoadFromFile(const std::string& folderPath, const std::string& fileName)
{
    _LastFilePath = folderPath + "/" + fileName;  // 경로 저장

    std::string content = DataManager::GetInstance()->LoadTextFile(folderPath, fileName);
    if (content.empty()) {
        // 에러 정보 저장
        _LastError = "Failed to load file: " + _LastFilePath;
        _HasError = true;
        return false;
    }

    _HasError = false;
    _LastError.clear();
    return LoadFromString(content);
}

bool AsciiArtRenderer::LoadFromString(const std::string& artContent)
{
    _ArtLines.clear();

    std::istringstream stream(artContent);
    std::string line;
    while (std::getline(stream, line)) {
        // \r 제거
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        _ArtLines.push_back(line);
    }

    if (_ArtLines.empty()) {
        _LastError = "Art content is empty";
        _HasError = true;
        _IsDirty = true;
        return false;
    }

    _HasError = false;
    _LastError.clear();
    _IsDirty = true;
    return !_ArtLines.empty();
}

bool AsciiArtRenderer::LoadAnimationFromFiles(const std::string& folderPath,
    const std::vector<std::string>& fileNames, float frameDuration)
{
    _AnimationFrames.clear();

    for (const auto& fileName : fileNames) {
        std::string content = DataManager::GetInstance()->LoadTextFile(folderPath, fileName);
        if (content.empty()) {
            PrintManager::GetInstance()->PrintLogLine(
                "AsciiArtRenderer: Failed to load animation frame " + fileName,
                ELogImportance::WARNING);
            continue;
        }

        std::vector<std::string> frame;
        std::istringstream stream(content);
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            frame.push_back(line);
        }

        _AnimationFrames.push_back(std::move(frame));
    }

    if (!_AnimationFrames.empty()) {
        _FrameDuration = frameDuration;
        _CurrentFrame = 0;
        _IsAnimating = true;
        _IsDirty = true;
        return true;
    }

    return false;
}

bool AsciiArtRenderer::LoadAnimationFromFolder(const std::string& animationFolderPath,
    float frameDuration,
    const std::string& extension)
{
    // 1. 폴더 내 모든 파일 목록 가져오기
    std::vector<std::string> fileNames =
        DataManager::GetInstance()->GetFilesInDirectory(animationFolderPath, extension);

    if (fileNames.empty())
    {
        PrintManager::GetInstance()->PrintLogLine(
            "AsciiArtRenderer: No files found in folder " + animationFolderPath,
            ELogImportance::WARNING);
        return false;
    }

    // 2. 기존 LoadAnimationFromFiles 호출
    return LoadAnimationFromFiles(animationFolderPath, fileNames, frameDuration);
}

void AsciiArtRenderer::Clear()
{
    _ArtLines.clear();
    _AnimationFrames.clear();
    _IsDirty = true;
}

void AsciiArtRenderer::Update(float deltaTime)
{
    if (!_IsAnimating || _AnimationFrames.empty()) return;

    _FrameTime += deltaTime;
    if (_FrameTime >= _FrameDuration) {
        _FrameTime = 0.0f;
        _CurrentFrame = (_CurrentFrame + 1) % _AnimationFrames.size();
        _IsDirty = true;
    }
}

void AsciiArtRenderer::Render(ScreenBuffer& buffer, const PanelBounds& bounds)
{
    // _IsDirty 체크 제거 - TextRenderer와 동일한 패턴

    // 애니메이션 모드
    if (_IsAnimating && !_AnimationFrames.empty()) {
        RenderFrame(buffer, bounds, _AnimationFrames[_CurrentFrame]);
    }
    // 단일 아트 모드
    else if (!_ArtLines.empty()) {
        RenderFrame(buffer, bounds, _ArtLines);
    }

    _IsDirty = false;
}

void AsciiArtRenderer::RenderFrame(ScreenBuffer& buffer, const PanelBounds& bounds,
    const std::vector<std::string>& frame)
{
    if (frame.empty()) return;

    int contentX = bounds.X + 1;
    int contentY = bounds.Y + 1;
    int contentWidth = bounds.Width - 2;
    int contentHeight = bounds.Height - 2;

    if (contentWidth <= 0 || contentHeight <= 0) return;

    // 아트 크기 검증 - 실제 화면 너비 계산 (UTF-8 고려)
    int artHeight = static_cast<int>(frame.size());
    int artWidth = 0;

    // 각 줄의 실제 화면 너비 계산
    for (const auto& line : frame) {
        int lineVisualWidth = 0;
        size_t i = 0;

        while (i < line.length()) {
            unsigned char ch = static_cast<unsigned char>(line[i]);
            int charLen = 1;
            int visualWidth = 1;

            // UTF-8 문자 너비 계산
            if (ch >= 0x80) {
                if ((ch & 0xF0) == 0xE0) {
                    charLen = 3;  // 한글, Box Drawing 등 (3바이트)
                    visualWidth = 2;
                }
                else if ((ch & 0xE0) == 0xC0) {
                    charLen = 2;  // 기타 유니코드
                    visualWidth = 2;
                }
                else if ((ch & 0xF8) == 0xF0) {
                    charLen = 4;  // 이모지 등
                    visualWidth = 2;
                }
            }

            lineVisualWidth += visualWidth;
            i += charLen;
        }

        artWidth = (std::max)(artWidth, lineVisualWidth);
    }

    // 패널보다 크면 경고
    if (artHeight > contentHeight || artWidth > contentWidth) {
        // 경고만 출력, 렌더링은 계속 진행
    }

    // 정렬 계산
    int startX = contentX;
    int startY = contentY;

    switch (_Alignment) {
    case ArtAlignment::CENTER:
        startX = contentX + (contentWidth - artWidth) / 2;
        startY = contentY + (contentHeight - artHeight) / 2;
        break;
    case ArtAlignment::RIGHT:
        startX = contentX + contentWidth - artWidth;
        break;
    case ArtAlignment::LEFT:
    default:
        break;
    }

    // 시작 위치가 음수면 보정
    if (startX < contentX) startX = contentX;
    if (startY < contentY) startY = contentY;

    // 렌더링
    int currentY = startY;
    for (const auto& line : frame) {
        if (currentY >= contentY + contentHeight) break;
        if (currentY >= bounds.Y + bounds.Height - 1) break;

        buffer.WriteString(startX, currentY, line, _Color);
        currentY++;
    }
}
