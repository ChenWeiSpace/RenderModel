#ifndef RenderContent_h__
#define RenderContent_h__
#include <memory>

class RenderContent : public std::enable_shared_from_this<RenderContent>
{
public:
	RenderContent();
	~RenderContent();
	void initDevice(int w,int h);
private:
	int m_width{0};
	int m_height{0};
};

#endif // RenderContent_h__
