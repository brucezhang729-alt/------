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

https://leetcode.cn/problems/sum-root-to-leaf-numbers/description/  // 求根节点到叶节点数字之和

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

https://leetcode.cn/problems/binary-tree-right-side-view/description/ //  二叉树右视图

class Solution {
public:
    vector<int> ret;
    vector<int> rightSideView(TreeNode* root) {
        dfs(root,0);
        return ret;
    }
    void dfs(TreeNode*root,int depth)
    {
        if(root==nullptr) return;
        if(ret.size()==depth) ret.push_back(root->val);
        dfs(root->right,depth+1);
        dfs(root->left,depth+1);
    }
};


https://leetcode.cn/problems/count-good-nodes-in-binary-tree/description/  //  统计二叉树中的好节点

class Solution {
public:
    int dfs(TreeNode*root,int Pathmax_)
    {
        if(root==nullptr) return 0;
        int ret = 0;
        if(root->val>=Pathmax_)
        {
            ret++;
            Pathmax_ = root->val;
        }
        ret += dfs(root->left,Pathmax_)+dfs(root->right,Pathmax_);
        return ret;
    }
    int goodNodes(TreeNode* root) {
        return dfs(root,INT_MIN);
    }
};