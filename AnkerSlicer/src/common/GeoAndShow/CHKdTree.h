/*

*



*

*/

#ifndef CHKdTree_H
#define CHKdTree_H


#include "QVector3D"



struct Range
{
	Range() {};
	Range(const int& start, const int& lenth)
	{
		m_start = start;
		m_lenth = lenth;
	}
    int m_start = -1;
	int m_lenth = 0;
};

class KDNode
{
public:
	KDNode();
	~KDNode();

public:
	KDNode* m_parent;
	KDNode* m_leftChild;
	KDNode* m_rightChild;
    int m_splitAxis;
    int m_index;
	Range m_range;
};

class CHKdTree
{
    struct IDPOINT  
    {
		QVector3D m_pt;
		int m_index;
		static bool pred1(const IDPOINT& a, const IDPOINT& b)
		{
			return a.m_pt[0] < b.m_pt[0];
		}
		static bool pred2(const IDPOINT& a, const IDPOINT& b)
		{
			return a.m_pt[1] < b.m_pt[1];
		}
		static bool pred3(const IDPOINT& a, const IDPOINT& b)
		{
			return a.m_pt[2] < b.m_pt[2];
		}
	};

    
	struct NODEHEAP
	{
		NODEHEAP() {};
		NODEHEAP(KDNode*& node, float& dis)
		{
			m_node = node;
			m_dis = dis;
		}

		bool operator<(const NODEHEAP& nd)
		{
			return m_dis < nd.m_dis;
		}

		KDNode* m_node = 0;
		float m_dis = 999999999;
	};

public:
	CHKdTree();
	~CHKdTree();

public:
	void createTree(const std::vector<QVector3D>& dataset);

    bool beEmpty();

	int getNearestPoint(QVector3D pt);

    
	void getKNearestPoint(QVector3D pt, int k, std::vector<int>& indexs);

private:
    
	void split(KDNode*& node, const Range& range, const int& splitAxis);

	void sortByAxis(const Range& range, const int& splitAxis);

	bool beInLeftChild(KDNode*& node, const QVector3D& pt);

    
	void searchTheWholeChildTree(KDNode*& node, const QVector3D& pt, float& nearestDistance,
		KDNode*& nearestNode);
	void searchTheWholeChildTree(KDNode*& node, const QVector3D& pt, std::vector<NODEHEAP>& nearestNodes);

private:

	std::vector<IDPOINT> m_dataset;
	KDNode* m_rootNode;

};

#endif
