#include "CHBVHTree.h"

using namespace  std;

BVHNode::BVHNode()
{
    m_leftChild = 0;
    m_rightChild = 0;
    m_index = -1;
}

BVHNode::~BVHNode()
{
    if (m_leftChild)
    {
        delete m_leftChild;
    }
    if (m_rightChild)
    {
        delete m_rightChild;
    }
}

bool BVHNode::beLeaf()
{
    return m_leftChild == 0 && m_rightChild == 0;
}

CHBVHTree::CHBVHTree()
{
    m_rootNode = 0;
}

CHBVHTree::~CHBVHTree()
{
    if (m_rootNode)
    {
        delete m_rootNode;
    }
}

void CHBVHTree::createTree(const vector<CHAABB3D>& data)
{
    if (m_rootNode)
    {
        delete m_rootNode;
    }

    int num = (int)data.size();
    if (num < 2)
    {
        return;
    }

    m_dataset.resize(num);
    CHAABB3D aabb;
    for (int i = 0; i < num; i++)
    {
        m_dataset[i].m_aabb = data[i];
        m_dataset[i].m_index = i;
        aabb.add(data[i]);
    }

    Range range(0, num);
#if 0
    if (num > 10000) 
    {
        
        m_rootNode = new BVHNode;
        m_rootNode->m_aabb = aabb;

        
        Range leftRange, rightRange;
        CHAABB3D leftAABB, rightAABB;
        splitAABB(range, aabb, leftRange, leftAABB, rightRange, rightAABB);
        m_rootNode->m_leftChild = new BVHNode;
        m_rootNode->m_leftChild->m_aabb = leftAABB;
        m_rootNode->m_rightChild = new BVHNode;
        m_rootNode->m_rightChild->m_aabb = rightAABB;

        
        Range rangel, range2, range3, range4;
        CHAABB3D aabb1, aabb2, aabb3, aabb4;
        splitAABB(leftRange, leftAABB, rangel, aabb1, range2, aabb2);
        splitAABB(rightRange, rightAABB, range3, aabb3, range4, aabb4);
        Range range5 = rangel;

        CFzTaskMgr taskMgr(4);
        taskMgr.DoTask(boost::bind(&CFzBVHTree::generateChildNodesByRecursion, this, boost::ref(m_rootNode->m_leftChild->m_leftChild),
                                   boost::ref(range5), boost::ref(aabb1)));
        taskMgr.DoTask(boost::bind(&CFzBVHTree::generateChildNodesByRecursion, this, boost::ref(m_rootNode->m_leftChild->m_rightChild),
                                   boost::ref(range2), boost::ref(aabb2)));
        taskMgr.DoTask(boost::bind(&CFzBVHTree::generateChildNodesByRecursion, this, boost::ref(m_rootNode->m_rightChild->m_leftChild),
                                   boost::ref(range3), boost::ref(aabb3)));
        taskMgr.DoTask(boost::bind(&CFzBVHTree::generateChildNodesByRecursion, this, boost::ref(m_rootNode->m_rightChild->m_rightChild),
                                   boost::ref(range4), boost::ref(aabb4)));
        taskMgr.Wait();
        taskMgr.Reset();
    }
    else
    {
        generateChildNodesByRecursion(m_rootNode, range, aabb);
    }
#endif
    generateChildNodesByRecursion(m_rootNode, range, aabb);
}

bool CHBVHTree::beEmpty()
{
    if (m_rootNode)
    {
        return false;
    }
    return true;
}

void CHBVHTree::pickByRay(const QVector3D& sp, const QVector3D& v, vector<int>& outobjs)
{
    if (!m_rootNode)
    {
        return;
    }

    
    searchByRay(m_rootNode, sp, v, outobjs);
}

void CHBVHTree::pickBySphere(const QVector3D& center, const float& squaredis, vector<int>& outobjs)
{
    if (!m_rootNode)
    {
        return;
    }

    
    searchByDisConstrain(m_rootNode, center, squaredis, outobjs);
}

void CHBVHTree::intersectPlane(const CHPlanePtr& plane, vector<int>& outobjs)
{
    if (!m_rootNode)
    {
        return;
    }

    
    searchByPlane(m_rootNode, plane, outobjs);
}

void CHBVHTree::generateChildNodesByRecursion(BVHNode*& node, const Range& range, const CHAABB3D& aabb)
{
    node = new BVHNode;
    node->m_aabb = aabb;

    if (range.m_lenth == 1)
    {
        node->m_index = m_dataset[range.m_start].m_index;
    }
    else
    {
        Range leftRange, rightRange;
        CHAABB3D leftAABB, rightAABB;
        splitAABB(range, aabb, leftRange, leftAABB, rightRange, rightAABB);

        generateChildNodesByRecursion(node->m_leftChild, leftRange, leftAABB);
        generateChildNodesByRecursion(node->m_rightChild, rightRange, rightAABB);
    }
}

void CHBVHTree::splitAABB(const Range& range, const CHAABB3D& aabb, Range& leftRange, CHAABB3D& leftAABB, Range& rightRange, CHAABB3D& rightAABB)
{
    int divNum = 10;
    int dim = aabb.getMaxLenthDim();
    float minCost = 9999999999;

    sortByAxis(range, dim);

    vector<float> checkAxisValues;
    if (range.m_lenth <= divNum)
    {
        checkAxisValues.resize(range.m_lenth - 1);
        for (int i = 0; i < range.m_lenth - 1; i++)
        {
            checkAxisValues[i] = m_dataset[range.m_start + i].m_aabb[dim];
        }
    }
    else
    {
        checkAxisValues.resize(divNum);

        
        float mincoord = m_dataset[range.m_start].m_aabb[dim];
        float maxcoord = m_dataset[range.m_start + range.m_lenth - 1].m_aabb[dim];
        float step = (maxcoord - mincoord) / divNum;
        for (int i = 0; i < divNum; i++)
        {
            checkAxisValues[i] = mincoord + i * step;
        }
    }

    
    for (int i = 0; i < (int)checkAxisValues.size(); i++)
    {
        

        
        
        
        //{
        //	if (m_dataset[range.m_start + p].m_aabb[dim] > checkAxisValues[i])
        //	{
        //		leftLenth = p;
        //		break;
        //	}
        //}

        int leftLenth = findInsertLocation(range.m_start, range.m_start + range.m_lenth - 1, dim, checkAxisValues[i]) -
                range.m_start + 1;

        Range tdata1, tdata2;
        tdata1.m_start = range.m_start;
        tdata1.m_lenth = leftLenth;
        tdata2.m_start = range.m_start + leftLenth;
        tdata2.m_lenth = range.m_lenth - leftLenth;
        CHAABB3D aabb1 = calAABB(tdata1);
        CHAABB3D aabb2 = calAABB(tdata2);

        float scale = 10000.0;
        float curCost = aabb1.getArea() * tdata1.m_lenth / scale + aabb2.getArea() * tdata2.m_lenth / scale;
        if (curCost < minCost)
        {
            minCost = curCost;
            leftRange = tdata1;
            rightRange = tdata2;
            leftAABB = aabb1;
            rightAABB = aabb2;
        }
    }
}

void CHBVHTree::sortByAxis(const Range& range, const int& dim)
{
    if (dim == 0)
    {
        std::sort(m_dataset.begin() + range.m_start, m_dataset.begin() + range.m_start + range.m_lenth /*- 1*/, IDOBJ::pred1);
    }
    else if (dim == 1)
    {
        std::sort(m_dataset.begin() + range.m_start, m_dataset.begin() + range.m_start + range.m_lenth/* - 1*/, IDOBJ::pred2);
    }
    else
    {
        std::sort(m_dataset.begin() + range.m_start, m_dataset.begin() + range.m_start + range.m_lenth /*- 1*/, IDOBJ::pred3);
    }
}

void CHBVHTree::searchByDisConstrain(BVHNode*& node, const QVector3D& center, const float& squaredis,
                                     vector<int>& outobjs)
{
    if (node->m_aabb.getMinSquareDis(center) <= squaredis)
    {
        if (node->beLeaf())
        {
            outobjs.push_back(node->m_index);
        }
        else
        {
            searchByDisConstrain(node->m_leftChild, center, squaredis, outobjs);
            searchByDisConstrain(node->m_rightChild, center, squaredis, outobjs);
        }
    }
}

void CHBVHTree::searchByRay(BVHNode*& node, const QVector3D& sp, const QVector3D& v, vector<int>& outobjs)
{
    if (node->m_aabb.isIntersectRay(sp, v))
    {
        if (node->beLeaf())
        {
            outobjs.push_back(node->m_index);
        }
        else
        {
            searchByRay(node->m_leftChild, sp, v, outobjs);
            searchByRay(node->m_rightChild, sp, v, outobjs);
        }
    }
}

void CHBVHTree::searchByPlane(BVHNode*& node, const CHPlanePtr& plane, vector<int>& outobjs)
{
    if (node->m_aabb.intersectWithPlane(plane))
    {
        if (node->beLeaf())
        {
            outobjs.push_back(node->m_index);
        }
        else
        {
            searchByPlane(node->m_leftChild, plane, outobjs);
            searchByPlane(node->m_rightChild, plane, outobjs);
        }
    }
}

CHAABB3D CHBVHTree::calAABB(const Range& range)
{
    CHAABB3D aabb;
    for (int i = 0; i < range.m_lenth; i++)
    {
        aabb.add(m_dataset[range.m_start + i].m_aabb);
    }
    return aabb;
}

int CHBVHTree::findInsertLocation(const int& startIndex, const int& endIndex, const int& dim, const float& value)
{
    int checkIndex = (int)((startIndex + endIndex) / 2);

    if (m_dataset[checkIndex + 1].m_aabb[dim] >= value && m_dataset[checkIndex].m_aabb[dim] <= value)
    {
        return checkIndex;
    }
    else if (m_dataset[checkIndex].m_aabb[dim] < value)
    {
        return findInsertLocation(checkIndex + 1, endIndex, dim, value);
    }
    else
    {
        return findInsertLocation(startIndex, checkIndex, dim, value);
    }
}


