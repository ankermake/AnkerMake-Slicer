/*



*

*/

#ifndef CHBVHTree_H
#define CHBVHTree_H


#include "CHAABB3D.h"
#include "CHKdTree.h"






class BVHNode
{
public:
	BVHNode();
	~BVHNode();

public:
	bool beLeaf();
public:
	CHAABB3D m_aabb;
	BVHNode* m_leftChild;
	BVHNode* m_rightChild;
	int m_index;
};

class CHBVHTree
{
    struct IDOBJ  
	{
		CHAABB3D m_aabb;
		int m_index;
		static bool pred1(const IDOBJ& a, const IDOBJ& b)
		{
			return a.m_aabb[0] < b.m_aabb[0];
		}
		static bool pred2(const IDOBJ& a, const IDOBJ& b)
		{
			return a.m_aabb[1] < b.m_aabb[1];
		}
		static bool pred3(const IDOBJ& a, const IDOBJ& b)
		{
			return a.m_aabb[2] < b.m_aabb[2];
		}
	};

public:
	CHBVHTree();
	~CHBVHTree();

public:
	void createTree(const std::vector<CHAABB3D>& data);

    bool beEmpty();

	void pickByRay(const QVector3D& sp, const QVector3D& v, std::vector<int>& outobjs);

    
	void pickBySphere(const QVector3D& center, const float& squaredis, std::vector<int>& outobjs);

	void intersectPlane(const CHPlanePtr& plane, std::vector<int>& outobjs);

private:
    
	void generateChildNodesByRecursion(BVHNode*& node, const Range& range, const CHAABB3D& aabb);
	void splitAABB(const Range& range, const CHAABB3D& aabb, Range& leftRange, CHAABB3D& leftAABB,
		Range& rightRange, CHAABB3D& rightAABB);

	void sortByAxis(const Range& range, const int& dim);

	void searchByDisConstrain(BVHNode*& node, const QVector3D& center, const float& squaredis, std::vector<int>& outobjs);
	void searchByRay(BVHNode*& node, const QVector3D& sp, const QVector3D& v, std::vector<int>& outobjs);
	void searchByPlane(BVHNode*& node, const CHPlanePtr& plane, std::vector<int>& outobjs);

	CHAABB3D calAABB(const Range& range);

    
    
	int findInsertLocation(const int& startIndex, const int& endIndex, const int& dim, const float& value);

private:
	std::vector<IDOBJ> m_dataset;
	BVHNode* m_rootNode;

};

#endif
