#include "CustomDrawNode.h"

USING_NS_CC;

bool CustomDrawNode::init()
{
	if (!DrawNode::init())
	{
		return false;
	}
	return true;
}

void CustomDrawNode::update(float dt)
{
	DrawNode::update(dt);
	double x1 = tail->getPositionX();//������Բ��ȷ�����ε�4����
	double y1 = tail->getPositionY();
	double x2 = head->getPositionX();
	double y2 = head->getPositionY();
	double R1 =tail->getScale() * 62;
	double R2 =head->getScale() * 62;
	clear();
	double dis = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	double Sin = (x1- x2) / dis;
	double Cos = (y1 - y2) / dis;
	tm += dt;

	double r = pattern?0.6:(0.6*(tm > atm ? 1 : tm/atm));
	Vec2 points[] = { Vec2(x1 - R1*Cos, y1 + R1*Sin), Vec2(x1 + R1*Cos, y1 - R1*Sin), Vec2(x2 + R2*Cos, y2 - R2*Sin), Vec2(x2 - R2*Cos, y2 + R2*Sin) };
	drawPolygon(points, sizeof(points) / sizeof(points[0]), Color4F(1, 1, 1, r), 0, Color4F(0, 0, 0, 0));
}