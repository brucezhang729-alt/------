https://leetcode.cn/problems/binary-tree-preorder-traversal/description/   前序遍历

class Solution {
public:
    void traversal(TreeNode*cur,vector<int>& vec)
    {
        if(cur==nullptr) return;
        vec.push_back(cur->val);
        traversal(cur->left,vec);
        traversal(cur->right,vec);
    }
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int>result;
        traversal(root,result);
        return result;
    }
};

https://leetcode.cn/problems/binary-tree-inorder-traversal/description/   中序遍历

class Solution {
public:
    void traveltree(TreeNode*cur,vector<int>&rec)
    {
        if(cur==nullptr) return;
        traveltree(cur->left,rec);
        rec.push_back(cur->val);
        traveltree(cur->right,rec);
    }
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int>result;
        traveltree(root,result);
        return result;
    }
};

https://leetcode.cn/problems/binary-tree-postorder-traversal/description/   后序遍历

class Solution {
public:
      void traveltree(TreeNode*cur,vector<int>&rec)
    {
        if(cur==nullptr) return;
        traveltree(cur->left,rec);
        traveltree(cur->right,rec);
         rec.push_back(cur->val);
    }
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int>result;
        traveltree(root,result);
        return result;
    }
};

https://leetcode.cn/problems/leaf-similar-trees/description/    872.叶子相似的树

class Solution {
public:
    void Leaf(TreeNode*root,vector<int>& res)
    {
        if(root==nullptr) return ;
        if(root->left==nullptr&&root->right==nullptr)
        {
            res.push_back(root->val);
            return;
        }
        Leaf(root->left,res);
        Leaf(root->right,res);
    }
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> res1,res2;
        Leaf(root1,res1);
        Leaf(root2,res2);
        return res1==res2;
    }
};

https://leetcode.cn/problems/sZ59z6/description/  LCP.44 开幕式烟火

class Solution {
public:
     unordered_map<int,int>map;
        void dfs(TreeNode* root)
        {
            if(!root) return ;
            map[root->val]++;
            dfs(root->left);
            dfs(root->right);
        }
    int numColor(TreeNode* root) {
       dfs(root);
       return map.size();
    }
};

https://leetcode.cn/problems/sum-of-left-leaves/description/   404.左叶子只和

class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if(!root) return 0;
        int leftValue = 0;
        if(root->left&&!root->left->left&&!root->left->right)
        {
            leftValue = root->left->val;
        }
        return leftValue+sumOfLeftLeaves(root->left)+sumOfLeftLeaves(root->right);
    }
};

https://leetcode.cn/problems/second-minimum-node-in-a-binary-tree/description/   671.二叉树中第二小的节点

class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        return dfs(root,root->val);
    }
    int dfs(TreeNode*root,int val)
    {
        if(root==nullptr) return -1;
         // 遇到大的直接返回，否则继续
        if(root->val>val) return root->val;
         // 找到左右子树中比val大一点的值
        int leftNum = dfs(root->left,val);
        int rightNum = dfs(root->right,val);
        // 如果都有大的，取小的
        if(leftNum>0&&rightNum>0)
        {
            return min(leftNum,rightNum);
        }
        // 其他情况都取大
        return max(leftNum,rightNum);
    }
};