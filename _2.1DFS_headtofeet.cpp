https://leetcode.cn/problems/maximum-depth-of-binary-tree/description/  // 二叉树的最大深度


class Solution {
public:
    int maxDepth(TreeNode* root) {
        if(root==nullptr) return 0;
        int leftdp = maxDepth(root->left);
        int rightdp = maxDepth(root->right);
        return max(leftdp,rightdp)+1;
    }
};

https://leetcode.cn/problems/minimum-depth-of-binary-tree/description/ //  二叉树的最小深度


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

https://leetcode.cn/problems/path-sum/description/  //  路径总和

class Solution {
public:
    bool dfs(TreeNode*root,int sum)
    {
        if(root==nullptr) return false;
        sum-=root->val;
        if(root->left==nullptr && root->right==nullptr)
            return sum==0;
        return dfs(root->left,sum)||dfs(root->right,sum);
    }
    bool hasPathSum(TreeNode* root, int targetSum) {
        return dfs(root,targetSum);
    }
};


class Solution {
public:
    int sum;  // 全局变量

    bool dfs(TreeNode* root)
    {
        if(root == nullptr) return false;

        sum -= root->val;

        if(root->left == nullptr && root->right == nullptr)
        {
            return sum == 0;
        }

        bool left = dfs(root->left);
        bool right = dfs(root->right);

        sum += root->val; // 回溯，恢复状态

        return left || right;
    }

    bool hasPathSum(TreeNode* root, int targetSum) {
        sum = targetSum;
        return dfs(root);
    }
};