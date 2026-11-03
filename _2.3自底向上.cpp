https://leetcode.cn/problems/univalued-binary-tree/description/  // 单值二叉树

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool isUnivalTree(TreeNode* root) {
        int _val = root->val;
        bool signal = true;
        dfs(root,signal,_val);
        return signal;
    }
    void dfs(TreeNode*root,bool& signal,int _val)
    {
        if(root==nullptr) return;
        if(root->val==_val)
        {
            dfs(root->left,signal,_val);
            dfs(root->right,signal,_val);
        }
        else
        {
            signal = false;
            return;
        }
    }
};

https://leetcode.cn/problems/same-tree/submissions/700150949/  //相同的树

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
  //都为空 
   if(p==NULL&q==NULL)
    {
        return true;
    }
    //其中一个空
    if(p==NULL||q==NULL)
    {
        return false;
    }
    //都不为空  结构相同
    if(p->val!=q->val)
    {
        return false;
    }
    //结构相同，值也相同
   return isSameTree(p->left,q->left)&&isSameTree(p->right,q->right);
}

https://leetcode.cn/problems/balanced-binary-tree/description/  // 平衡二叉树

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int height(TreeNode*root)
    {
        if(root==nullptr) return 0;
        else return max(height(root->left),height(root->right))+1;
    }
    bool isBalanced(TreeNode* root) {
        if(root==nullptr) return true;
        else return abs(height(root->left)-height(root->right))<=1 && isBalanced(root->left) && isBalanced(root->right);
    }
};

https://leetcode.cn/problems/evaluate-boolean-binary-tree/description/  // 计算二叉树的布尔值

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool evaluateTree(TreeNode* root) {
        if(root->left==nullptr) return root->val == 0?false:true;
        bool left = evaluateTree(root->left);
        bool right = evaluateTree(root->right);
        return root->val==2?left|right:left&right;
    }
};

https://leetcode.cn/problems/symmetric-tree/description/  // 对称的树

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool check(TreeNode*P,TreeNode*Q)
    {
        if(!P&&!Q) return true;
        if(!P||!Q) return false;
        return P->val == Q->val && check(P->left,Q->right) && check(P->right,Q->left);
    }
    bool isSymmetric(TreeNode* root) {
        return check(root->left, root->right);
    }
};

https://leetcode.cn/problems/flip-equivalent-binary-trees/submissions/756734309/  // 翻转等价二叉树

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool flipEquiv(TreeNode* root1, TreeNode* root2) {
        if(!root1&&!root2) return true;
        if(!root1||!root2) return false;
        if(root1->val!=root2->val) return false;
        return flipEquiv(root1->left,root2->left)&&flipEquiv(root1->right,root2->right)||flipEquiv(root1->left,root2->right)&&flipEquiv(root1->right,root2->left);
    }
};

https://leetcode.cn/problems/most-frequent-subtree-sum/description/  // 出现次数最多的子树元素和

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    unordered_map<int,int> count;
    int Maxcount = 0;
    vector<int> findFrequentTreeSum(TreeNode* root) {
        count.clear();
        Maxcount = 0;
        dfs(root);

        vector<int> ret;
        for(auto&[num,frequency]:count)
        {
            if(frequency==Maxcount)
            {
                ret.push_back(num);
            }
        }
        return ret;
    }
    int dfs(TreeNode*root)
    {
        if(root==nullptr) return 0;
        int left = dfs(root->left);
        int right = dfs(root->right);
        int sum = root->val+left+right;

        count[sum]++;
        Maxcount = max(Maxcount,count[sum]);
        return sum;
    }
};

https://leetcode.cn/problems/binary-tree-tilt/description/  // 二叉树的坡度

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int sum = 0;
    int findTilt(TreeNode* root) {
        dfs(root);
        return sum;
    }
    int dfs(TreeNode*root)
    {
        if(root==nullptr) return 0;
        int left = dfs(root->left);
        int right = dfs(root->right);
        sum += abs(left-right);
        return left+right+root->val;
    }
};


https://leetcode.cn/problems/count-dominant-nodes-in-a-binary-tree/description/  // 统计二叉树中支配节点的个数

class Solution {
public:
    int ret = 0;
    int countDominantNodes(TreeNode* root) {
        int maxval = 0;
        dfs(root,maxval);
        return  ret;     
    }
    int dfs(TreeNode*root,int val)
    {
        if(root==nullptr) return 0;
        val = max(dfs(root->left,val),dfs(root->right,val));
        if(root->val>=val)
        {
            ret++;
            val = root->val;
        }
       return val;
    }
};

https://leetcode.cn/problems/construct-string-from-binary-tree/description/  // 从二叉树创建字符串

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    string tree2str(TreeNode* root) {
        if(!root) return "";
        string ret = to_string(root->val);
        if(root->left)
        {
            ret+="("+tree2str(root->left)+")";
        }
        else if(root->right)
        {
            ret+="()";
        }
        if(root->right)
        {
            ret+="("+tree2str(root->right)+")";
        }
        return ret;

    }
};

https://leetcode.cn/problems/count-nodes-equal-to-average-of-subtree/description/  // 统计与子树平均值相等的节点数

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    int ret = 0;
    int averageOfSubtree(TreeNode* root) {
        dfs(root);
        return ret;
    }
    pair<int,int> dfs(TreeNode*root)
    {
        if(root==nullptr) return {0,0};
        // 计算左右子树
        auto left = dfs(root->left);
        auto right = dfs(root->right);
         // 统计当前子树的总和与节点数量
        int presum = left.first+right.first+root->val;
        int precount = left.second+right.second+1;
        int average = presum/precount;
        // 更新
        if(average==root->val) ret++;
        // 返回当前子树和与节点个数
        return {presum,precount};
    }
};


https://leetcode.cn/problems/maximum-difference-between-node-and-ancestor/description/ // 节点与其祖先之间的最大差值

class Solution {
public:
    int MaxDiffer = 0;
    // 记录一条路径上的最大值和最小值
    int maxAncestorDiff(TreeNode* root) {
        int max = 0,min = INT_MAX/2;
        dfs(root,max,min);
        return MaxDiffer;
    }
    void dfs(TreeNode*root,int max,int min)
    {
        if(root==nullptr) return ;
        if(root->val>max) max = root->val;
        if(root->val<min) min = root->val;
        MaxDiffer = MaxDiffer>(max-min)?MaxDiffer:(max-min);
        dfs(root->left,max,min);
        dfs(root->right,max,min);
    }
};

https://leetcode.cn/problems/binary-tree-coloring-game/description/ // 二叉树着色游戏

class Solution {
public:
    bool btreeGameWinningMove(TreeNode* root, int n, int x) {
        // 先找到x所在的位置，定位为递归入口 然后找节点个数
        TreeNode* cur = nullptr;
        dfs1(root,x,cur);
        int left = dfs(cur->left);
        int right = dfs(cur->right);
        int parent = n-left-right-1;
        return max({left,right,parent})>n/2;
    }
    void dfs1(TreeNode*root,int x,TreeNode*&cur)
    {
        if(root==nullptr) return;
        if(root->val==x)
        {
            cur = root;
            return;
        }
        dfs1(root->left,x,cur);
        dfs1(root->right,x,cur);
    }
    int dfs(TreeNode*root)
    {
        if(root==nullptr) return 0 ;
        int left = dfs(root->left);
        int right = dfs(root->right);
        return left+right+1;
    }
};