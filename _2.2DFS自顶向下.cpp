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

https://leetcode.cn/problems/binary-tree-right-side-view/description/ // 二叉树右视图

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

https://leetcode.cn/problems/count-good-nodes-in-binary-tree/  // 二叉树中好节点个数

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

https://leetcode.cn/problems/sum-of-nodes-with-even-valued-grandparent/description/  祖父节点值为偶数的节点和

class Solution {
public:
    int sumEvenGrandparent(TreeNode* root) {
        return dfs(root,nullptr,nullptr);
    }
    int dfs(TreeNode*root,TreeNode*parent,TreeNode*grandparent)
    {
        if(root==nullptr) return 0;

        int sum = 0;
         // 如果祖父节点存在且值为偶数
        if(grandparent&&grandparent->val%2==0)
        {
            sum+=root->val;
        }
        sum+=dfs(root->left,root,parent)+dfs(root->right,root,parent);
        return sum;
    }
};

https://leetcode.cn/problems/smallest-string-starting-from-leaf/description/  // 从叶子节点开始的最小字符串

class Solution {
public:
    vector<string> path;
    string smallestFromLeaf(TreeNode* root) {
        dfs(root,"");
        sort(path.begin(),path.end());
        return path[0];
    }
    void dfs(TreeNode*root,string s)
    {
        if(root==nullptr) return ;
        s+='a'+root->val;
        if(root->left==nullptr&&root->right==nullptr)
        {
            reverse(s.begin(),s.end()); // 从叶子节点到根节点
            path.push_back(s);
            return;
        }
        dfs(root->left,s);
        dfs(root->right,s);
    }
};

https://leetcode.cn/problems/maximum-difference-between-node-and-ancestor/description/ // 节点与其祖先节点最大的差值

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