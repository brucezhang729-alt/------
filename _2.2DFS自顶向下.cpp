https://leetcode.cn/problems/maximum-depth-of-binary-tree/description/  // 二叉树最大深度

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if(root==nullptr) return 0;
        int leftdp = maxDepth(root->left);
        int rightdp = maxDepth(root->right);
        return max(leftdp,rightdp)+1;
    }
};

https://leetcode.cn/problems/minimum-depth-of-binary-tree/description/  // 二叉树最小深度

class Solution {
public:
    int minDepth(TreeNode* root) {
        if(root==nullptr) return 0;
        if(root->left==nullptr&&root->right==nullptr) return 1;

        int Min_Dep = INT_MAX/2;
        if(root->left) Min_Dep = min(minDepth(root->left),Min_Dep);
        if(root->right) Min_Dep = min(minDepth(root->right),Min_Dep);

        return Min_Dep+1;        
    }
};

https://leetcode.cn/problems/path-sum/description/  // 求根节点到叶子结点数字之和

class Solution {
public:
    int dfs(TreeNode*root,int presum)
    {
        presum = presum*10+root->val;
        if(root->left==nullptr&&root->right==nullptr)
        {
            return presum;
        }
        int ret = 0;
        if(root->left) ret+=dfs(root->left,presum);
        if(root->right) ret+=dfs(root->right,presum);
        return ret;
    }
    int sumNumbers(TreeNode* root) {
        return dfs(root,0);
    }
};