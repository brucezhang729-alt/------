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