#include <d3d9.h>

namespace gui
{
	inline bool open = true;
	inline bool setup = false;
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = {};
	inline WNDPROC originalWindowProcess = nullptr;

	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool setupWindowClass(const char* windowClassName) noexcept;
	void destroyWindowClass() noexcept;

	bool setupWindow(const char* windowName) noexcept;
	void destroyWindow() noexcept;

	bool setupDirectX() noexcept;
	void destoryDirextX() noexcept;

	void xsetup();

	void setupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void destroy() noexcept;

	void render() noexcept;
}