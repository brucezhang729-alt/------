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

https://leetcode.cn/problems/sum-of-root-to-leaf-binary-numbers/description/ // 从根节点到叶子节点的二进制数和

class Solution {
public:
    int SUM = 0;

    int sumRootToLeaf(TreeNode* root) {
        SUM = 0;
        dfs(root, 0);
        return SUM;
    }

    void dfs(TreeNode* root, int num) {
        if (root == nullptr) return;

        // 将当前节点加入二进制数
        num = num * 2 + root->val;

        // 到达叶节点，累加结果
        if (root->left == nullptr && root->right == nullptr) {
            SUM += num;
            return;
        }

        dfs(root->left, num);
        dfs(root->right, num);
    }
};


https://leetcode.cn/problems/add-one-row-to-tree/description/ // 在二叉树中添加一行

class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        if(root==nullptr) return nullptr;
        if(depth==1)
        {
            return new TreeNode(val,root,nullptr);
        }
        if(depth==2)
        {
            root->left = new TreeNode(val,root->left,nullptr);
            root->right = new TreeNode(val,nullptr,root->right);
        }
        else
        {
            root->left = addOneRow(root->left,val,depth-1);
            root->right = addOneRow(root->right,val,depth-1);
        }
        return root;
    }
};

https://leetcode.cn/problems/longest-zigzag-path-in-a-binary-tree/submissions/756202244/  // 二叉树最长交错路径

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
    int longestZigZag(TreeNode* root) {
        dfs(root,nullptr,0);
        return ret;
    }
    void dfs(TreeNode*root,TreeNode*parent,int length)
    {
        if(root==nullptr) return;   
        // 1.到达一个新的节点，更新结果
        ret = max(length,ret);
        // 2.如果parent为空，从root开始
        if(parent==nullptr)
        {
            dfs(root->left,root,1);
            dfs(root->right,root,1);
        }
        // 3.如果上一步是走左
        else if(parent->left==root)
        {
            dfs(root->right,root,length+1); // 交叉路径 length+1
            dfs(root->left,root,1); // 延续同方向，长度length置为1
            
        }
        // 4. 如果上一步走右
        else
        {
            dfs(root->left,root,length+1); //...
            dfs(root->right,root,1); //...
        }
    }
};

https://leetcode.cn/problems/flip-binary-tree-to-match-preorder-traversal/description/  // 翻转二叉树以匹配先序遍历值

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
    vector<int> flipMatchVoyage(TreeNode* root, vector<int>& voyage) {
        vector<int> ret;
        int index = 0;
        if(!dfs(root,voyage,ret,index)) return {-1};
        return ret;
    }
    bool dfs(TreeNode*root,vector<int>&voyage,vector<int>&ret,int& index)
    {
        if(root==nullptr) return true ;
        // 当前节点必须匹配先序遍历值
        if(index>=voyage.size()||root->val!=voyage[index])
        {
            return false;
        }
        // 节点匹配 准备遍历下一个节点
        index++;
        // 如果下一个预期节点不是左孩子 尝试反转
        if(root->left!=nullptr&&index<voyage.size()&&root->left->val!=voyage[index])
        {
            ret.push_back(root->val);
            // 反转后：先访问右子树 在访问左子树
            return dfs(root->right,voyage,ret,index)&&dfs(root->left,voyage,ret,index);
        }    
        return dfs(root->left,voyage,ret,index)&&dfs(root->right,voyage,ret,index);
    }
};