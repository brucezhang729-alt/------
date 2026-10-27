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