#include "CHKdTree.h"
#include "CHAABB3D.h"

using namespace std;

KDNode::KDNode()
{
	m_parent = 0;
	m_leftChild = 0;
	m_rightChild = 0;
	m_splitAxis = 0;
	m_index = 0;
}

KDNode::~KDNode()
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

CHKdTree::CHKdTree()
{
	m_rootNode = 0;
}

CHKdTree::~CHKdTree()
{
	if (m_rootNode)
	{
		delete m_rootNode;
	}
}

void CHKdTree::createTree(const vector<QVector3D>& dataset)
{
	if (m_rootNode)
	{
		delete m_rootNode;
	}

    
    
	CHAABB3D aabb;

	int lenth = (int)dataset.size();
	m_dataset.resize(lenth);
	for (int i = 0; i < lenth; i++)
	{
		aabb.add(dataset[i][0], dataset[i][1], dataset[i][2]);
		m_dataset[i].m_pt = dataset[i];
		m_dataset[i].m_index = i;
	}

    int splitAxis;
	if (aabb.getLenX() > aabb.getLenY() && aabb.getLenX() > aabb.getLenZ())
	{
		splitAxis = 0;
	}
	else if (aabb.getLenY() > aabb.getLenX() && aabb.getLenY() > aabb.getLenZ())
	{
		splitAxis = 1;
	}
	else
	{
		splitAxis = 2;
	}
	Range range(0, lenth);
	split(m_rootNode, range, splitAxis);
}

bool CHKdTree::beEmpty()
{
	if (m_rootNode)
	{
		return false;
	}
	return true;
}

int CHKdTree::getNearestPoint(QVector3D pt)
{
	if (!m_rootNode)
	{
		return -1;
	}

    
    vector<KDNode*> allnodes;
	KDNode* curNode = m_rootNode;
    while (curNode)
	{
		allnodes.push_back(curNode);
		if (beInLeftChild(curNode, pt))
		{
			curNode = curNode->m_leftChild;
		}
		else
		{
			curNode = curNode->m_rightChild;
		}
	}

	//??
	KDNode* nearestNode;
    float nearestDistance = 999999999;
	while ((int)allnodes.size() != 0)
	{
		KDNode* checkNode = allnodes.back();
		allnodes.pop_back();

		if (fabs(pt[checkNode->m_splitAxis] - m_dataset[checkNode->m_index].m_pt[checkNode->m_splitAxis]) <
            nearestDistance)
		{
			float tlenth = (m_dataset[checkNode->m_index].m_pt - pt).length();
            if (tlenth < nearestDistance)
			{
				nearestNode = checkNode;
				nearestDistance = tlenth;
			}

			KDNode* childNode;
			if (beInLeftChild(checkNode, pt))
			{
				childNode = checkNode->m_rightChild;
			}

			else
			{
				childNode = checkNode->m_leftChild;
			}

            
            
            
			searchTheWholeChildTree(childNode, pt, nearestDistance, nearestNode);
		}
        
		//{
		//	break;
		//}
	}

	return m_dataset[nearestNode->m_index].m_index;
}

void CHKdTree::getKNearestPoint(QVector3D pt, int k, vector<int>& indexs)
{
	if (k < 1)
	{
		return;
	}

    
    vector<KDNode*> allnodes;
	KDNode* curNode = m_rootNode;
    while (curNode)
	{
		allnodes.push_back(curNode);
		if (beInLeftChild(curNode, pt))
		{
			curNode = curNode->m_leftChild;
		}
		else
		{
			curNode = curNode->m_rightChild;
		}
	}

	vector<NODEHEAP> nearestNodes(k);
	make_heap(nearestNodes.begin(), nearestNodes.end());

	while ((int)allnodes.size() > 0)
	{
		KDNode* checkNode = allnodes.back();
		allnodes.pop_back();

        float nearestDistance = nearestNodes[0].m_dis;
		if (fabs(pt[checkNode->m_splitAxis] - m_dataset[checkNode->m_index].m_pt[checkNode->m_splitAxis]) <
            nearestDistance)
		{
			float tlenth = (m_dataset[checkNode->m_index].m_pt - pt).length();
            if (tlenth < nearestDistance)
			{
                
				pop_heap(nearestNodes.begin(), nearestNodes.end());
				nearestNodes.pop_back();

                
				nearestNodes.push_back(NODEHEAP(checkNode, tlenth));
				push_heap(nearestNodes.begin(), nearestNodes.end());
			}

			KDNode* childNode;
			if (beInLeftChild(checkNode, pt))
			{
				childNode = checkNode->m_rightChild;
			}
			else
			{
				childNode = checkNode->m_leftChild;
			}

            
            
            
			searchTheWholeChildTree(childNode, pt, nearestNodes);
		}
	}

	indexs.resize(k);
	for (int i = 0; i < k; i++)
	{
		if (nearestNodes[i].m_node)
		{
			indexs[i] = m_dataset[nearestNodes[i].m_node->m_index].m_index;
		}
	}
}

void CHKdTree::split(KDNode*& node, const Range& range, const int& splitAxis)
{
	if (range.m_lenth == 0)
	{
		node = 0;
	}
	else if (range.m_lenth == 1)
	{
		node = new KDNode;
		node->m_range = range;
		node->m_splitAxis = splitAxis;
		node->m_index = range.m_start;
	}
	else
	{
		node = new KDNode;
		node->m_range = range;
		node->m_splitAxis = splitAxis;

        
		sortByAxis(range, splitAxis);

        int newlenth = (int)(range.m_lenth / 2);
		node->m_index = range.m_start + newlenth;
		Range leftRange(range.m_start, newlenth);
		Range rightRange(range.m_start + newlenth + 1, range.m_lenth - newlenth - 1);
        int newsplitAxis = (splitAxis + 1) % 3;
		split(node->m_leftChild, leftRange, newsplitAxis);
		split(node->m_rightChild, rightRange, newsplitAxis);
		if (node->m_leftChild)
		{
			node->m_leftChild->m_parent = node;
		}
		if (node->m_rightChild)
		{
			node->m_rightChild->m_parent = node;
		}
	}
}

void CHKdTree::sortByAxis(const Range& range, const int& splitAxis)
{
	if (splitAxis == 0)
	{
		std::sort(m_dataset.begin() + range.m_start, m_dataset.begin() + range.m_start + range.m_lenth, IDPOINT::pred1);
	}
	else if (splitAxis == 1)
	{
		std::sort(m_dataset.begin() + range.m_start, m_dataset.begin() + range.m_start + range.m_lenth, IDPOINT::pred2);
	}
	else
	{
		std::sort(m_dataset.begin() + range.m_start, m_dataset.begin() + range.m_start + range.m_lenth, IDPOINT::pred3);
	}
}

bool CHKdTree::beInLeftChild(KDNode*& node, const QVector3D& pt)
{
	return pt[node->m_splitAxis] <= m_dataset[node->m_index].m_pt[node->m_splitAxis];
}

void CHKdTree::searchTheWholeChildTree(KDNode*& node, const QVector3D& pt,
	float& nearestDistance, KDNode*& nearestNode)
{
	if (node)
	{
		if (fabs(pt[node->m_splitAxis] - m_dataset[node->m_index].m_pt[node->m_splitAxis]) < nearestDistance)
		{
			float tlenth = (m_dataset[node->m_index].m_pt - pt).length();
            if (tlenth < nearestDistance)
			{
				nearestNode = node;
				nearestDistance = tlenth;
			}
			searchTheWholeChildTree(node->m_leftChild, pt, nearestDistance, nearestNode);
			searchTheWholeChildTree(node->m_rightChild, pt, nearestDistance, nearestNode);
		}
        else  
		{
			KDNode* searchChildNode;
			if (beInLeftChild(node, pt))
			{
				searchChildNode = node->m_leftChild;
			}
			else
			{
				searchChildNode = node->m_rightChild;
			}
			searchTheWholeChildTree(searchChildNode, pt, nearestDistance, nearestNode);
		}
	}
}

void CHKdTree::searchTheWholeChildTree(KDNode*& node, const QVector3D& pt, vector<NODEHEAP>& nearestNodes)
{
	if (node)
	{
        float nearestDistance = nearestNodes[0].m_dis;
		if (fabs(pt[node->m_splitAxis] - m_dataset[node->m_index].m_pt[node->m_splitAxis]) < nearestDistance)
		{
			float tlenth = (m_dataset[node->m_index].m_pt - pt).length();
            if (tlenth < nearestDistance)
			{
                
				pop_heap(nearestNodes.begin(), nearestNodes.end());
				nearestNodes.pop_back();

                
				nearestNodes.push_back(NODEHEAP(node, tlenth));
				push_heap(nearestNodes.begin(), nearestNodes.end());
			}
			searchTheWholeChildTree(node->m_leftChild, pt, nearestNodes);
			searchTheWholeChildTree(node->m_rightChild, pt, nearestNodes);
		}
        else  
		{
			KDNode* searchChildNode;
			if (beInLeftChild(node, pt))
			{
				searchChildNode = node->m_leftChild;
			}
			else
			{
				searchChildNode = node->m_rightChild;
			}
			searchTheWholeChildTree(searchChildNode, pt, nearestNodes);
		}
	}
}

