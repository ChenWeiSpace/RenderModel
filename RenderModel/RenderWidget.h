#ifndef RenderWidget_h__
#define RenderWidget_h__
#include <memory>
#include <QWidget>
#include <QTime>

typedef std::shared_ptr<class RenderContent> RenderContentPtr;

class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	RenderWidget(QWidget *parent);
	~RenderWidget();
public:
	void buildContent();
protected:
	void timerEvent(QTimerEvent *event) override;
private:
	RenderContentPtr m_renderConten;
	std::uint64_t m_current{ 0 };
	QTime m_time;
	std::uint64_t m_pressTime{ 0 };
};
#endif // RenderWidget_h__
