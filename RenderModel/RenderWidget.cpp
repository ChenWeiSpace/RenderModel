#include "RenderWidget.h"
#include "RenderContent.h"

RenderWidget::RenderWidget(QWidget *parent)
	: QWidget(parent)
{
}

RenderWidget::~RenderWidget()
{
}

void RenderWidget::buildContent()
{
	auto winId = this->winId();
	QSize size = this->size();
	RenderContentPtr rendCon = std::make_shared<RenderContent>();
	winId = 0;//render to texture
	rendCon->initDevice(winId, size.width(), size.height());
	m_renderConten = rendCon;
	if (winId  )
	{
		this->startTimer(20);
	}
	else
	{
		rendCon->frameMove(0, 0);
	}
}

void RenderWidget::timerEvent(QTimerEvent *event)
{
	static std::uint64_t frameNumber = 0;
	std::uint64_t val = m_time.elapsed();
	if (m_current == 0)
	{
		m_current = val;
	}
	m_renderConten->frameMove(frameNumber++, val - m_current);
	m_current = val;
}
