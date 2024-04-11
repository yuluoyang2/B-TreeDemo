/*
*参考资料： https://oi-wiki.org/ds/bplus-tree/ 
*/
#include<iostream>
#include <string>
#include<queue>
#include<algorithm>
using namespace std;
const int ORDER_V = 2;//设为4阶 最小则为2阶
const int MAXNUM_KEY = 2 * ORDER_V;
const int MAXNUM_POINTER = (MAXNUM_KEY + 1);//指针个数
const int MAXNUM_DATA = 2 * ORDER_V;
typedef int Key_Type;
#define NULL 0
#define INVALID 0

#define FLAG_LEFT 1
#define FLAG_RIGHT 2
std::string value_words[1001];
//节点状态
enum Node_Type
{
	Node_Type_Root = 1,
	Node_Type_Internal = 2,
	Node_Type_leaf = 3,
};
/*
* 节点类型 内部节点和叶子节点的父类
* 属性： 节点类型，父节点指针，节点内部数据个数
* 方法： 构建函数和析构函数 set、get方法，pointer用于中间结点
*		 获取兄弟节点 删除孩子节点
*/
class CNode
{
public:
	CNode();
	virtual ~CNode();
	//获取和设置节点类型
	Node_Type GetType() {
		return m_Type;
	}
	void SetType(Node_Type type) {
		m_Type = type;
	}

	//获取和设置有效数据个数
	int GetCount() {
		return m_Count;
	}
	void SetCount(int count) {
		m_Count = count;
	}

	//获取和设置某个元素，对中间结点指键值，对叶子结点指数据
	virtual Key_Type GetElement(int i) {
		return 0;
	}
	virtual void SetElement(int i, Key_Type type) {}

	//获取和设置某个指针，对中间结点指指针，对叶子结点无意义
	virtual CNode* GetPointer(int i) { return nullptr; }
	virtual void SetPointer(int i, CNode* pointer) {}

	// 获取和设置父结点
	CNode* GetFather() { return m_pFather; }
	void SetFather(CNode* father) { m_pFather = father; }

	//获取最近的兄弟节点
	CNode* GetBrother(int& flag);
	void DeleteChildren();
protected:
	Node_Type m_Type;//节点类型
	int m_Count;//有效数据个数
	CNode* m_pFather;
};
//构建函数和析构函数
CNode::CNode()
{
	m_Type = Node_Type_leaf;
	m_Count = 0;
	m_pFather = NULL;
}
CNode::~CNode()
{
	DeleteChildren();
}
//获取兄弟节点 flag表示在兄弟节点在当前节点的左边还是右边
CNode* CNode::GetBrother(int& flag)
{
	CNode* pFather = GetFather();   //获取其父结点指针
	if (NULL == pFather)
	{
		return NULL;
	}

	CNode* pBrother = NULL;

	for (int i = 1; i <= pFather->GetCount() + 1; i++)   //GetCount()表示获取数据或关键字数，要比指针数小1。
	{
		// 找到本结点的位置
		if (pFather->GetPointer(i) == this)
		{
			if (i == (pFather->GetCount() + 1))   //表示其为父结点的最右边孩子。
			{
				pBrother = pFather->GetPointer(i - 1);    // 本身是最后一个指针，只能找前一个指针
				flag = FLAG_LEFT;
			}
			else
			{
				pBrother = pFather->GetPointer(i + 1);    // 优先找后一个指针
				flag = FLAG_RIGHT;
			}
		}
	}

	return pBrother;
}
void CNode::DeleteChildren()   
{
	for (int i = 1; i <= GetCount(); i++)   //GetCount()为返回结点中关键字即数据的个数
	{
		CNode* pNode = GetPointer(i);
		if (NULL != pNode)    // 叶子结点没有指针
		{
			pNode->DeleteChildren();
		}

		delete pNode;
	}
}
/// <summary>
/// 内部节点数据结构 继承CNode
/// 属性： Key数组和指针数组 
/// 方法： 重写set和get方法 insert插入 Split分裂 Combine结合 MoveOneElement移动
/// </summary>
class CInternalNode :public CNode {
public:
	CInternalNode();
	virtual ~CInternalNode();
	Key_Type GetElement(int i) {
		if (i > 0 && i <= MAXNUM_KEY) {
			return m_Keys[i - 1];
		}
		else
		{
			return INVALID;
		}
	}
	void SetElement(int i, Key_Type key) {
		if (i > 0 && i <= MAXNUM_KEY) {
			m_Keys[i - 1] = key;
		}
	}
	CNode* GetPointer(int i) {
		if (i > 0 && i <= MAXNUM_POINTER) {
			return m_Pointers[i - 1];
		}
		else
		{
			return NULL;
		}
	}
	void SetPointer(int i, CNode* pointer) {
		if (i > 0 && i <= MAXNUM_POINTER) {
			m_Pointers[i - 1] = pointer;
		}
	}
	bool Insert(Key_Type value, CNode* pNode);
	bool Delete(Key_Type value);
	//分裂节点
	Key_Type Split(CInternalNode* pNode, Key_Type key);
	//结合节点
	bool Combine(CNode* pNode);
	//从另一节点移动一个元素到本节点
	bool MoveOneElement(CNode* pNode);
protected:
	Key_Type m_Keys[MAXNUM_KEY];
	CNode* m_Pointers[MAXNUM_POINTER];
};
//构建和析构
CInternalNode::CInternalNode()
{
	m_Type = Node_Type_Internal;

	int i = 0;
	for (i = 0; i < MAXNUM_KEY; i++)
	{
		m_Keys[i] = INVALID;
	}

	for (i = 0; i < MAXNUM_POINTER; i++)
	{
		m_Pointers[i] = NULL;
	}
}
CInternalNode::~CInternalNode()
{
	for (int i = 0; i < MAXNUM_POINTER; i++)
	{
		m_Pointers[i] = NULL;
	}
}
bool CInternalNode::Insert(Key_Type value, CNode* pNode)
{
	int i;
	// 如果中间结点已满，直接返回失败
	if (GetCount() >= MAXNUM_KEY)
	{
		return false;
	}

	int j = 0;

	// 找到要插入键的位置
	for (i = 0; (value > m_Keys[i]) && (i < m_Count); i++)
	{
	}

	// 当前位置及其后面的键依次后移，空出当前位置
	for (j = m_Count; j > i; j--)
	{
		m_Keys[j] = m_Keys[j - 1];
	}

	// 当前位置及其后面的指针依次后移
	for (j = m_Count + 1; j > i + 1; j--)
	{
		m_Pointers[j] = m_Pointers[j - 1];
	}

	// 把键和指针存入当前位置
	m_Keys[i] = value;
	m_Pointers[i + 1] = pNode;    // 注意是第i+1个指针而不是第i个指针
	pNode->SetFather(this);      // 非常重要  该函数的意思是插入关键字value及其所指向子树

	m_Count++;

	// 返回成功
	return true;
}

// 在中间结点中删除键，以及该键后的指针
bool CInternalNode::Delete(Key_Type key)
{
	int i, j, k;
	for (i = 0; (key >= m_Keys[i]) && (i < m_Count); i++)
	{
	}

	for (j = i - 1; j < m_Count - 1; j++)
	{
		m_Keys[j] = m_Keys[j + 1];
	}
	m_Keys[j] = INVALID;

	for (k = i; k < m_Count; k++)
	{
		m_Pointers[k] = m_Pointers[k + 1];
	}
	m_Pointers[k] = NULL;

	m_Count--;

	return true;
}
/// <summary>
/// 1.如果要插入的键值在第V和V+1个键值中间，需要翻转一下，因此先处理此情况
/// 2.处理key小于第V个键值或key大于第V+1个键值的情况
/// </summary>
/// <param name="pNode"></param>
/// <param name="key"></param>
/// <returns></returns>
Key_Type CInternalNode::Split(CInternalNode* pNode, Key_Type key)  //key是新插入的值，pNode是分裂结点
{
	int i = 0, j = 0;
	if ((key > this->GetElement(ORDER_V)) && (key < this->GetElement(ORDER_V + 1)))
	{
		// 把第V+1 -- 2V个键移到指定的结点中

		for (i = ORDER_V + 1; i <= MAXNUM_KEY; i++)
		{
			j++;
			pNode->SetElement(j, this->GetElement(i));
			this->SetElement(i, INVALID);
		}

		// 把第V+2 -- 2V+1个指针移到指定的结点中
		j = 0;
		for (i = ORDER_V + 2; i <= MAXNUM_POINTER; i++)
		{
			j++;
			this->GetPointer(i)->SetFather(pNode);    // 重新设置子结点的父亲
			pNode->SetPointer(j, this->GetPointer(i));
			this->SetPointer(i, INVALID);
		}

		// 设置好Count个数
		this->SetCount(ORDER_V);
		pNode->SetCount(ORDER_V);

		// 把原键值返回
		return key;
	}

	// 判断是提取第V还是V+1个键
	int position = 0;
	if (key < this->GetElement(ORDER_V))
	{
		position = ORDER_V;
	}
	else
	{
		position = ORDER_V + 1;
	}

	// 把第position个键提出来，作为新的键值返回
	Key_Type RetKey = this->GetElement(position);

	// 把第position+1 -- 2V个键移到指定的结点中
	j = 0;
	for (i = position + 1; i <= MAXNUM_KEY; i++)
	{
		j++;
		pNode->SetElement(j, this->GetElement(i));
		this->SetElement(i, INVALID);
	}

	// 把第position+1 -- 2V+1个指针移到指定的结点中(注意指针比键多一个)
	j = 0;
	for (i = position + 1; i <= MAXNUM_POINTER; i++)
	{
		j++;
		this->GetPointer(i)->SetFather(pNode);    // 重新设置子结点的父亲
		pNode->SetPointer(j, this->GetPointer(i));
		this->SetPointer(i, INVALID);
	}

	// 清除提取出的位置
	this->SetElement(position, INVALID);

	// 设置好Count个数
	this->SetCount(position - 1);
	pNode->SetCount(MAXNUM_KEY - position);


	return RetKey;
}
bool CInternalNode::Combine(CNode* pNode)
{
	// 参数检查
	if (this->GetCount() + pNode->GetCount() + 1 > MAXNUM_DATA)    // 预留一个新键的位置
	{
		return false;
	}

	// 取待合并结点的第一个孩子的第一个元素作为新键值
	Key_Type NewKey = pNode->GetPointer(1)->GetElement(1);  

	m_Keys[m_Count] = NewKey;
	m_Count++;
	m_Pointers[m_Count] = pNode->GetPointer(1);  

	for (int i = 1; i <= pNode->GetCount(); i++)
	{
		m_Keys[m_Count] = pNode->GetElement(i);
		m_Count++;
		m_Pointers[m_Count] = pNode->GetPointer(i + 1);
	}
	return true;
}
bool CInternalNode::MoveOneElement(CNode* pNode)
{
	// 参数检查
	if (this->GetCount() >= MAXNUM_DATA)
	{
		return false;
	}

	int i, j;

	// 兄弟结点在本结点左边
	if (pNode->GetElement(1) < this->GetElement(1))
	{
		// 先腾出位置
		for (i = m_Count; i > 0; i--)
		{
			m_Keys[i] = m_Keys[i - 1];
		}
		for (j = m_Count + 1; j > 0; j--)
		{
			m_Pointers[j] = m_Pointers[j - 1];
		}

		// 赋值
		// 第一个键值不是兄弟结点的最后一个键值，而是本结点第一个子结点的第一个元素的值
		m_Keys[0] = GetPointer(1)->GetElement(1);
		// 第一个子结点为兄弟结点的最后一个子结点
		m_Pointers[0] = pNode->GetPointer(pNode->GetCount() + 1);

		// 修改兄弟结点
		pNode->SetElement(pNode->GetCount(), INVALID);
		pNode->SetPointer(pNode->GetCount() + 1, INVALID);
	}
	else    // 兄弟结点在本结点右边
	{
		// 赋值
		// 最后一个键值不是兄弟结点的第一个键值，而是兄弟结点第一个子结点的第一个元素的值
		m_Keys[m_Count] = pNode->GetPointer(1)->GetElement(1);
		// 最后一个子结点为兄弟结点的第一个子结点
		m_Pointers[m_Count + 1] = pNode->GetPointer(1);

		// 修改兄弟结点
		for (i = 1; i < pNode->GetCount() - 1; i++)
		{
			pNode->SetElement(i, pNode->GetElement(i + 1));
		}
		for (j = 1; j < pNode->GetCount(); j++)
		{
			pNode->SetPointer(j, pNode->GetPointer(j + 1));
		}
	}

	// 设置数目
	this->SetCount(this->GetCount() + 1);
	pNode->SetCount(pNode->GetCount() - 1);

	return true;
}
class CLeafNode :public CNode {
public:
	CLeafNode();
	virtual ~CLeafNode();
	Key_Type GetElement(int i) {
		if (i > 0 && i <= MAXNUM_DATA) {
			return m_Datas[i - 1];
		}
		else
		{
			return INVALID;
		}
	}
	void SetElement(int i, Key_Type data) {
		if (i > 0 && i <= MAXNUM_DATA) {
			m_Datas[i - 1] = data;
		}
	}
	CNode* GetPointer(int i) {
		return NULL;
	}
	// 插入数据
	bool Insert(Key_Type value);
	// 删除数据
	bool Delete(Key_Type value);

	// 分裂结点
	Key_Type Split(CNode* pNode);
	// 结合结点
	bool Combine(CNode* pNode);
public:
	CLeafNode* Pre_CNode;
	CLeafNode* Next_CNode;
protected:
	Key_Type m_Datas[MAXNUM_DATA];
};
CLeafNode::CLeafNode()
{
	m_Type = Node_Type_leaf;

	for (int i = 0; i < MAXNUM_DATA; i++)
	{
		m_Datas[i] = INVALID;
	}

	Pre_CNode = NULL;
	Next_CNode = NULL;
}
CLeafNode::~CLeafNode()
{

}
bool CLeafNode::Insert(Key_Type value)
{
	int i, j;
	// 如果叶子结点已满，直接返回失败
	if (GetCount() >= MAXNUM_DATA)
	{
		return false;
	}

	// 找到要插入数据的位置
	for (i = 0; (value > m_Datas[i]) && (i < m_Count); i++)
	{
	}

	// 当前位置及其后面的数据依次后移，空出当前位置
	for (j = m_Count; j > i; j--)
	{
		m_Datas[j] = m_Datas[j - 1];
	}

	// 把数据存入当前位置
	m_Datas[i] = value;

	m_Count++;

	// 返回成功
	return true;
}
bool CLeafNode::Delete(Key_Type value)
{
	int i, j;
	bool found = false;
	for (i = 0; i < m_Count; i++)
	{
		if (value == m_Datas[i])
		{
			found = true;
			break;
		}
	}
	// 如果没有找到，返回失败
	if (false == found)
	{
		return false;
	}

	// 后面的数据依次前移
	for (j = i; j < m_Count - 1; j++)
	{
		m_Datas[j] = m_Datas[j + 1];
	}

	m_Datas[j] = INVALID;
	m_Count--;

	// 返回成功
	return true;

}
Key_Type CLeafNode::Split(CNode* pNode)
{
	// 把本叶子结点的后一半数据移到指定的结点中
	int j = 0;
	for (int i = ORDER_V + 1; i <= MAXNUM_DATA; i++)
	{
		j++;
		pNode->SetElement(j, this->GetElement(i));
		this->SetElement(i, INVALID);
	}
	// 设置好Count个数
	this->SetCount(this->GetCount() - j);
	pNode->SetCount(pNode->GetCount() + j);

	// 返回新结点的第一个元素作为键
	return pNode->GetElement(1);
}
bool CLeafNode::Combine(CNode* pNode)
{
	// 参数检查
	if (this->GetCount() + pNode->GetCount() > MAXNUM_DATA)
	{
		return false;
	}

	for (int i = 1; i <= pNode->GetCount(); i++)
	{
		this->Insert(pNode->GetElement(i));
	}

	return true;
}
class BPlusTree {
public:
	BPlusTree();
	virtual ~BPlusTree();
	bool Search(Key_Type data, string& sPath);
	bool Insert(Key_Type data);
	bool Delete(Key_Type data);
	void ClearTree();
	void PrintTree();
	//BPlusTree* RotateTree();
	//bool CheckTree();
	void PrintNode(CNode* pNode);
	CNode* GetRoot() {
		return m_Root;
	}
	void SetRoot(CNode* Root) {
		m_Root = Root;
	}
	int GetDepth() {
		return m_Depth;
	}
	void SetDepth(int depth) {
		m_Depth = depth;
	}
	void IncDepth()
	{
		m_Depth = m_Depth + 1;
	}

	// 深度减一
	void DecDepth()
	{
		if (m_Depth > 0)
		{
			m_Depth = m_Depth - 1;
		}
	}
public:
	CLeafNode* m_pLeafHead;
	CLeafNode* m_pLeafTail;
protected:
	CLeafNode* SearchLeafNode(Key_Type data);
	bool InsertInternalNode(CInternalNode* pNode, Key_Type key, CNode* pRightSon);
	bool DeleteInternalNode(CInternalNode* pNode, Key_Type key);
	CNode* m_Root;
	int m_Depth;
};
BPlusTree::BPlusTree()
{
	m_Depth = 0;
	m_Root = NULL;
	m_pLeafHead = NULL;
	m_pLeafTail = NULL;
}
BPlusTree::~BPlusTree()
{
	ClearTree();
}
void BPlusTree::ClearTree()
{
	CNode* pNode = GetRoot();
	if (NULL != pNode)
	{
		pNode->DeleteChildren();

		delete pNode;
	}

	m_pLeafHead = NULL;
	m_pLeafTail = NULL;
	SetRoot(NULL);
}
bool BPlusTree::Search(Key_Type data, string& sPath)
{
	int i = 0;
	sPath = "查找路径为： ";
	CNode* p = GetRoot();
	if (p == NULL) {
		return false;
	}
	//CInternalNode* q;
	while (p != NULL)
	{
		if (p->GetType() == Node_Type_leaf) {
			break;
		}
		for (i = 1; i <= p->GetCount() && data >= p->GetElement(i); i++);
		//if()
		int k = i > 1 ? i - 1 : i;
		sPath += to_string(p->GetElement(k));
		sPath += "-->";
		p = p->GetPointer(i);
	}
	if (p == NULL) {
		return false;
	}
	sPath += to_string(p->GetElement(1));
	bool found = false;
	for (i = 1; i <= p->GetCount(); i++) {
		if (data == p->GetElement(i)) {
			found = true;
		}
	}
	if (found) {
		sPath =sPath+ " OK"+" "+"return: " + value_words[data];
	}
	else
	{
		sPath += " Fail";

	}
	return found;
}
/* 在B+树中插入数据
插入数据首先要找到理论上要插入的叶子结点，然后分三种情况：
(1) 叶子结点未满。直接在该结点中插入即可；
(2) 叶子结点已满，且无父结点(即根结点是叶子结点)，需要首先把叶子结点分裂，然后选择插入原结点或新结点，然后新生成根节点；
(3) 叶子结点已满，但其父结点未满。需要首先把叶子结点分裂，然后选择插入原结点或新结点，再修改父结点的指针；
(4) 叶子结点已满，且其父结点已满。需要首先把叶子结点分裂，然后选择插入原结点或新结点，接着把父结点分裂，再修改祖父结点的指针。
	因为祖父结点也可能满，所以可能需要一直递归到未满的祖先结点为止。
*/
bool BPlusTree::Insert(Key_Type data)  //
{
	// 检查是否重复插入
	string a;
	bool found = Search(data, a);
	if (true == found)
	{
		return false;
	}
	// 查找理想的叶子结点
	CLeafNode* pOldNode = SearchLeafNode(data);
	// 如果没有找到，说明整个树是空的，生成根结点
	if (NULL == pOldNode)
	{
		pOldNode = new CLeafNode;
		m_pLeafHead = pOldNode;
		m_pLeafTail = pOldNode;
		SetRoot(pOldNode);
	}
	// 叶子结点未满，对应情况1，直接插入
	if (pOldNode->GetCount() < MAXNUM_DATA)
	{
		return pOldNode->Insert(data);
	}

	// 原叶子结点已满，新建叶子结点，并把原结点后一半数据剪切到新结点
	CLeafNode* pNewNode = new CLeafNode;
	Key_Type key = INVALID;
	key = pOldNode->Split(pNewNode);

	// 在双向链表中插入结点
	CLeafNode* pOldNext = pOldNode->Next_CNode;
	pOldNode->Next_CNode = pNewNode;
	pNewNode->Next_CNode = pOldNext;
	pNewNode->Pre_CNode = pOldNode;
	if (NULL == pOldNext)
	{
		m_pLeafTail = pNewNode;
	}
	else
	{
		pOldNext->Pre_CNode = pNewNode;
	}


	// 判断是插入到原结点还是新结点中，确保是按数据值排序的
	if (data < key)
	{
		pOldNode->Insert(data);    // 插入原结点
	}
	else
	{
		pNewNode->Insert(data);    // 插入新结点
	}

	// 父结点
	CInternalNode* pFather = (CInternalNode*)(pOldNode->GetFather());

	// 如果原结点是根节点，对应情况2
	if (NULL == pFather)
	{
		CNode* pNode1 = new CInternalNode;
		pNode1->SetPointer(1, pOldNode);                           // 指针1指向原结点
		pNode1->SetElement(1, key);                                // 设置键
		pNode1->SetPointer(2, pNewNode);                           // 指针2指向新结点
		pOldNode->SetFather(pNode1);                               // 指定父结点
		pNewNode->SetFather(pNode1);                               // 指定父结点
		pNode1->SetCount(1);

		SetRoot(pNode1);                                           // 指定新的根结点
		return true;
	}

	// 情况3和情况4在这里实现
	bool ret = InsertInternalNode(pFather, key, pNewNode);
	return ret;
}
bool BPlusTree::Delete(Key_Type data)
{
	CLeafNode* pOldNode = SearchLeafNode(data);
	if (pOldNode == NULL) {
		return false;
	}
	bool success = pOldNode->Delete(data);
	if (!success) {
		return false;
	}
	CInternalNode* pFather = (CInternalNode*)(pOldNode->GetFather());
	//一个数据都没有了
	if (pFather == NULL) {
		if (pOldNode->GetCount() == 0) {
			delete pOldNode;
			m_pLeafHead = NULL;
			m_pLeafTail = NULL;
			SetRoot(NULL);
		}
		return true;
	}
	/*找到存储有该关键字所在的结点时，由于该结点中关键字个数大于⌈M/2⌉，
	做删除操作不会破坏 B+树，则可以直接删除
	*/
	if (pOldNode->GetCount() >= ORDER_V) {
		for (int i = 1; data >= pFather->GetElement(i) && i <= pFather->GetCount(); i++) {
			// 如果删除的是父结点的键值，需要更改该键
			if (pFather->GetElement(i) == data) {
				pFather->SetElement(i, pOldNode->GetElement(1));
			}
		}
		return true;
	}
	int flag = FLAG_LEFT;
	CLeafNode* pBrother = (CLeafNode*)(pOldNode->GetBrother(flag));
	Key_Type NewData = INVALID;
	/*
	如果删除后叶子结点填充度<50%，需要先找到一个最近的兄弟结点(左右均可)，然后分两种情况：
	如果该兄弟结点填充度>50%，把该兄弟结点的最近一个数据剪切到本结点，父结点的键值也要
	相应修改。
	如果该兄弟结点的填充度=50%，则把两个结点合并，父结点键也相应合并。
	(如果合并后父结点的填充度<50%，则需要递归)*/
	if (pBrother->GetCount() > ORDER_V) {
		if (flag == FLAG_LEFT) {
			NewData = pBrother->GetElement(pBrother->GetCount());
		}
		else {
			NewData = pBrother->GetElement(1);
		}
		pOldNode->Insert(NewData);
		pBrother->Delete(NewData);
		//替换parent中的key值
		if (flag == FLAG_LEFT) {
			for (int i = 1; i <= pFather->GetCount() + 1; i++) {
				if (pFather->GetPointer(i) == pOldNode && i > 1)
				{
					pFather->SetElement(i - 1, pOldNode->GetElement(1));
					// 更改本结点对应的键
				}
			}
		}
		else
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pOldNode && i > 1)
				{
					pFather->SetElement(i - 1, pOldNode->GetElement(1));
					// 更改本结点对应的键
				}
				if (pFather->GetPointer(i) == pBrother && i > 1)
				{
					pFather->SetElement(i - 1, pBrother->GetElement(1));
					// 更改兄弟结点对应的键
				}
			}
		}
		return true;
	}
	Key_Type NewKey = NULL;
	if (FLAG_LEFT == flag) {
		pBrother->Combine(pOldNode);
		NewKey = pOldNode->GetElement(1);
		CLeafNode* OldNext = pOldNode->Next_CNode;
		pBrother->Next_CNode = OldNext;
		if (OldNext != NULL) {
			OldNext->Pre_CNode = pBrother;
		}
		else
		{
			m_pLeafTail = pBrother;
		}
		delete pOldNode;
	}
	else
	{
		pOldNode->Combine(pBrother);
		NewKey = pOldNode->GetElement(1);
		CLeafNode* OldNext = pBrother->Next_CNode;
		pOldNode->Next_CNode = OldNext;
		if (OldNext != NULL) {
			OldNext->Pre_CNode = pOldNode;
		}
		else
		{
			m_pLeafTail = pOldNode;
		}
		delete pBrother;
	}
	return DeleteInternalNode(pFather, NewKey);
}
// 查找对应的叶子结点
CLeafNode* BPlusTree::SearchLeafNode(Key_Type data)
{
	int i = 0;

	CNode* pNode = GetRoot();
	// 循环查找对应的叶子结点
	while (NULL != pNode)
	{
		// 结点为叶子结点，循环结束
		if (Node_Type_leaf == pNode->GetType())
		{
			break;
		}

		// 找到第一个键值大于等于key的位置
		for (i = 1; i <= pNode->GetCount(); i++)
		{
			if (data < pNode->GetElement(i))
			{
				break;
			}
		}

		pNode = pNode->GetPointer(i);
	}

	return (CLeafNode*)pNode;
}

//递归函数：插入键到中间结点
bool BPlusTree::InsertInternalNode(CInternalNode* pNode, Key_Type key, CNode* pRightSon)
{
	if (NULL == pNode || Node_Type_leaf == pNode->GetType())
	{
		return false;
	}

	// 结点未满，直接插入
	if (pNode->GetCount() < MAXNUM_KEY)
	{
		return pNode->Insert(key, pRightSon);
	}

	CInternalNode* pBrother = new CInternalNode;  //C++中new 类名表示分配一个类需要的内存空间，并返回其首地址；
	Key_Type NewKey = INVALID;
	// 分裂本结点
	NewKey = pNode->Split(pBrother, key);

	if (pNode->GetCount() < pBrother->GetCount())
	{
		pNode->Insert(key, pRightSon);
	}
	else if (pNode->GetCount() > pBrother->GetCount())
	{
		pBrother->Insert(key, pRightSon);
	}
	else    // 两者相等，即键值在第V和V+1个键值中间的情况，把字节点挂到新结点的第一个指针上
	{
		pBrother->SetPointer(1, pRightSon);
		pRightSon->SetFather(pBrother);
	}

	CInternalNode* pFather = (CInternalNode*)(pNode->GetFather());
	// 直到根结点都满了，新生成根结点
	if (NULL == pFather)
	{
		pFather = new CInternalNode;
		pFather->SetPointer(1, pNode);                           // 指针1指向原结点
		pFather->SetElement(1, NewKey);                          // 设置键
		pFather->SetPointer(2, pBrother);                        // 指针2指向新结点
		pNode->SetFather(pFather);                               // 指定父结点
		pBrother->SetFather(pFather);                            // 指定父结点
		pFather->SetCount(1);

		SetRoot(pFather);                                        // 指定新的根结点
		return true;
	}

	// 递归
	return InsertInternalNode(pFather, NewKey, pBrother);
}

// 递归函数：在中间结点中删除键
bool BPlusTree::DeleteInternalNode(CInternalNode* pNode, Key_Type key)
{
	// 删除键，如果失败一定是没有找到，直接返回失败
	bool success = pNode->Delete(key);
	if (false == success)
	{
		return false;
	}

	// 获取父结点
	CInternalNode* pFather = (CInternalNode*)(pNode->GetFather());
	if (NULL == pFather)
	{
		// 如果一个数据都没有了，把根结点的第一个结点作为根结点
		if (0 == pNode->GetCount())
		{
			SetRoot(pNode->GetPointer(1));
			delete pNode;
		}

		return true;
	}

	// 删除后结点填充度仍>=Order
	if (pNode->GetCount() >= ORDER_V)
	{
		for (int i = 1; (key >= pFather->GetElement(i)) && (i <= pFather->GetCount()); i++)
		{
			// 如果删除的是父结点的键值，需要更改该键
			if (pFather->GetElement(i) == key)
			{
				pFather->SetElement(i, pNode->GetElement(1));    
				// 更改为叶子结点新的第一个元素
			}
		}

		return true;
	}

	//找到一个最近的兄弟结点(根据B+树的定义，除了根结点，总是能找到的)
	int flag = FLAG_LEFT;
	CInternalNode* pBrother = (CInternalNode*)(pNode->GetBrother(flag));

	// 兄弟结点填充度>Order
	Key_Type NewData = INVALID;
	if (pBrother->GetCount() > ORDER_V)
	{
		pNode->MoveOneElement(pBrother);

		// 修改父结点的键值
		if (FLAG_LEFT == flag)
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pNode && i > 1)
				{
					pFather->SetElement(i - 1, pNode->GetElement(1));    // 更改本结点对应的键
				}
			}
		}
		else
		{
			for (int i = 1; i <= pFather->GetCount() + 1; i++)
			{
				if (pFather->GetPointer(i) == pNode && i > 1)
				{
					pFather->SetElement(i - 1, pNode->GetElement(1));    // 更改本结点对应的键
				}
				if (pFather->GetPointer(i) == pBrother && i > 1)
				{
					pFather->SetElement(i - 1, pBrother->GetElement(1));    // 更改兄弟结点对应的键
				}
			}
		}

		return true;
	}

	// 父结点中要删除的键：兄弟结点都不大于order，则需要合并结点，此时父结点需要删除键
	Key_Type NewKey = NULL;

	// 把本结点与兄弟结点合并，无论如何合并到数据较小的结点，这样父结点就无需修改指针
	if (FLAG_LEFT == flag)
	{
		(void)pBrother->Combine(pNode);
		NewKey = pNode->GetElement(1);
		delete pNode;
	}
	else
	{
		(void)pNode->Combine(pBrother);
		NewKey = pBrother->GetElement(1);
		delete pBrother;
	}

	// 递归
	return DeleteInternalNode(pFather, NewKey);
}

void BPlusTree::PrintTree()
{
	CNode* p = GetRoot();
	if (p == NULL) {
		return;
	}
	CInternalNode* a;
	int H = 1;
	queue<CNode*> q;
	queue<int> h;
	q.push(p);
	h.push(2);
	while (!q.empty())
	{
		p = q.front();
		if (H != h.front()) {
			cout << endl;
			cout << H << endl;
			H = h.front();
		}
		q.pop();
		h.pop();
		PrintNode(p);
		if (p != NULL && p->GetType() != Node_Type_leaf) {
			a = (CInternalNode*)p;
			for (int i = 1; i <= MAXNUM_POINTER; i++)
			{
				q.push(a->GetPointer(i));
				h.push(H + 1);
			}
		}
	}
}
void BPlusTree::PrintNode(CNode* pNode)
{
	if (pNode == NULL) {
		return;
	}
	for (int i = 1; i <= MAXNUM_KEY; i++) {
		cout << "(" << pNode->GetElement(i) << "," << value_words[pNode->GetElement(i)] << ")" << "  ";
		if (i >= MAXNUM_KEY) {
			cout << "||";
		}
	}
	cout << endl;
}
void select_one(BPlusTree* pTree, int count) {
	srand((unsigned)time(NULL));
	cout << "插入的数据为" << endl;
	for (int i = 1; i <= count; i++) {
		int x = rand() % 1000 + 1;
		value_words[x] = " ";
		for (int i = 0; i < 2; i++) {
			int y = rand() % 26 + 65;//生成一个随机数
			int z = rand() % 26 + 97;
			value_words[x] += char(y);
			value_words[x] += char(z);
		}
		cout << "[" << x << " "
			<< "," << value_words[x] << "]" << " ";
		pTree->Insert(x);//x=key
	}
	cout << "B+树建立成功!" << endl;
	cout << endl;
}
void select_two(BPlusTree* pTree, int count) {
	string sPath;
	pTree->Search(count, sPath);
	cout << sPath << endl;
}
void select_three(BPlusTree* pTree, int count,string s) {
	bool success = pTree->Insert(count);
	//value_words[count] = s;
	if (success == true) {
		value_words[count] = s;
		cout << "OK" << endl;
	}
	else
	{
		cout << "FAIL" << endl;
	}
	cout << endl;
}
void select_four(BPlusTree* pTree, int count) {
	bool success = pTree->Delete(count);
	if (true == success) {
		cout << "Success!" << endl;
		value_words[count] = "";
	}
	else
	{
		cout << "Error" << endl;
	}
	cout << endl;
}
void select_five(BPlusTree* pTree) {
	pTree->PrintTree();
	cout << endl;
}
void select_six(BPlusTree* pTree,int key1,string value1,int key2,string value2) {
	//cout << "功能正在开发" << endl;
	if (key1 == key2) {
		value_words[key1] = value2;
	}
	else
	{
		pTree->Delete(key1);
		pTree->Insert(key2);
		value_words[key2] = value2;
	}
	cout << endl;
}
int main() {
	BPlusTree* pTree = new BPlusTree();
	int x = 1;
	int y = 0;
	int i, k;
	value_words[0] = "NULL";
	string s = " ";
	string j = " ";
	string l = " ";
	while (x != 0)
	{
		cout << "1.随机建立N个节点的B+tree" << endl;
		cout << "2.需要查询 " << endl;
		cout << "3.需要插入内容 " << endl;
		cout << "4.需要删除内容" << endl;
		cout << "5.需要显示B+树形状" << endl;
		cout << "6.需要修改内容" << endl;
		cout << "0.退出程序" << endl;
		cout << "请输入你的选择 \n";
		cin >> x;
		switch (x)
		{
		case 1:
			cout << "请输入数据个数:" << endl;
			cin >> y;
			select_one(pTree, y);
			break;
		case 2:
			cout << "请输入要查询的KEY值:" << endl;
			cin >> y;
			select_two(pTree, y);
			break;
		case 3:
			cout << "请输入要插入的KEY值和Value" << endl;
			cout << "请先输入key：";
			cin >> y;
			cout << "请输入value:";
			cin >> s;
			select_three(pTree, y, s);
			break;
		case 4:
			cout << "请输入要删除的数值" << endl;
			cin >> y;
			select_four(pTree, y);
			break;
		case 5:
			select_five(pTree);
			break;
		case 6:
			cout << "请输入要删除的key和value" << endl;
			cout << "请先输入key：";
			cin >> i;
			cout << "请输入value:";
			cin >> j;
			cout << "请输入要增加的key和value" << endl;
			cout << "请先输入key：";
			cin >> k;
			cout << "请输入value:";
			cin >> l;
			select_six(pTree,i, j, k, l);
			break;
		case 0:
			delete pTree;
			return 0;
			break;
		default:
			break;
		}
	}
	delete pTree;
	return 0;
}