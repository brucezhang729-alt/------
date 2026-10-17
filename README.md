# 递归、搜索与回溯：从板书到二叉树解题方法

> 学习主线：**找到相同的子问题 → 设计函数头 → 只讨论一层函数体 → 确定递归出口 → 画图核对参数、返回值和状态恢复。**
>
> 本文以 `algorithm-master.zip` 中「02. 递归、搜索与回溯算法_题目列表+板书」为基准。板书给出分析框架，本文补全函数含义、代码、图解和边界讨论；再用[灵茶山艾府的题单](https://leetcode.cn/discuss/post/3142882/fen-xiang-gun-ti-dan-lian-biao-er-cha-sh-6srp/)中的二叉树题做迁移。代码为重新编写的 C++17 示例，不是压缩包中的代码文件。

验证说明：20 个完整 `Solution` 示例已从本文直接提取，以 MSVC 的 C++17 模式编译，通过 3,952 项检查；包括基础边界、重复调用、350 组随机小树及 200 组随机网格对照。讨论用伪代码与未展开的进阶题不在这项编译验证范围内。

## 目录

- [1. 如何对应原始板书](#1-如何对应原始板书)
- [2. 先分清递归、DFS、搜索、回溯、剪枝](#2-先分清递归dfs搜索回溯剪枝)
- [3. 一道题的递归讨论顺序](#3-一道题的递归讨论顺序)
- [4. 决策树、调用树与返回过程怎么画](#4-决策树调用树与返回过程怎么画)
- [5. 板书六题：逐题建立函数契约](#5-板书六题逐题建立函数契约)
- [6. 从板书迁移到二叉树题单](#6-从板书迁移到二叉树题单)
- [7. 从二叉树走向排列、子集和组合](#7-从二叉树走向排列子集和组合)
- [8. 网格搜索：哪些标记要恢复](#8-网格搜索哪些标记要恢复)
- [9. 记忆化搜索：什么才算相同的子问题](#9-记忆化搜索什么才算相同的子问题)
- [10. 题型辨认与练习路线](#10-题型辨认与练习路线)
- [11. 常见错误与检查方法](#11-常见错误与检查方法)
- [12. 可复制的讨论模板与自测](#12-可复制的讨论模板与自测)

## 1. 如何对应原始板书

所有板书文件均位于压缩包的以下目录，表中用编号和题目简称定位：

```text
algorithm-master/
└── 02. 递归、搜索与回溯算法_题目列表+板书/
    ├── README.md
    └── 递归、搜索与回溯算法_板书/
        ├── 00_名词解释+答疑+课程介绍.jpg
        ├── 06_计算布尔二叉树的值.jpg
        ├── ...
        └── 41_矩阵中的最长递增路径.jpg
```

| 板书 | 本文保留的分析重点 | 对应章节 |
|---|---|---|
| 00 名词解释、01 汉诺塔 | 相同子问题；函数头、函数体、递归出口；宏观理解与细节展开 | 2—4 |
| 06 计算布尔二叉树的值 | 先求左右子问题，再组合结果 | 5.1 |
| 07 求根节点到叶节点数字之和 | `dfs(root, presum)`；前缀向下传，结果向上加 | 5.2 |
| 08 二叉树剪枝 | 后序处理；返回处理后的子树根 | 5.3 |
| 09 验证二叉搜索树、10 第 k 小 | 中序顺序；共享变量；失败或找到答案后提前结束 | 5.4—5.5 |
| 11 二叉树的所有路径 | 路径、答案、恢复现场 | 5.6 |
| 12 全排列、13 子集 | 先画决策树；选项、状态、递归出口；子集的两种画法 | 4、7 |
| 15 全排列 II、20 组合总和 | 同层去重；合法分支；由决策树确定递归参数 | 7 |
| 26 单词搜索、29 FloodFill 简介、31 岛屿数量 | 四方向搜索；路径访问标记与全局访问标记 | 8 |
| 37 斐波那契数、41 矩阵中的最长递增路径 | 备忘录；状态、转移、出口之间的对应关系 | 9 |

**内容边界：**第 5 节直接围绕六张二叉树板书展开；第 6 节是结合外部题单所作的拓展。本文没有逐题覆盖压缩包所有专题，也不复刻外部题单的完整题目列表。

**语言约定：**板书出现 C++/Java 写法，本文统一用 C++17，突出 `TreeNode*`、按值传参、引用和共享状态。所有 `Solution` 代码块分别对应一道题，分别使用；刷力扣时节点定义由平台提供。

<!-- example: support -->
```cpp
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    explicit TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};
```

后文 `n` 表示节点数或输入元素数，`h` 表示树高，`L` 表示全部输出内容的总长度。树高统一按节点数计算：空树高 0，叶子高 1。直径单独按**边数**计算。

## 2. 先分清递归、DFS、搜索、回溯、剪枝

板书先讲名词，是为了让你看到它们各自解决什么问题。

| 概念 | 关注的问题 | 在代码中的表现 |
|---|---|---|
| 递归 | 如何用同类、规模更小的子问题完成任务？ | 函数调用自身，最终到达边界 |
| 搜索 | 有哪些可能的状态或方案？ | 从初始状态不断产生后继状态 |
| DFS，深度优先搜索 | 以什么次序探索？ | 先沿一个分支深入，再返回处理其他分支 |
| BFS，广度优先搜索 | 以什么次序探索？ | 通常用队列，一层一层展开 |
| 回溯 | 当前选择试完后，如何继续尝试其他选择？ | 撤销当前分支对共享状态的影响 |
| 剪枝 | 哪些分支无需继续？ | 证明无解、重复或已足够后跳过分支 |
| 记忆化搜索 | 同一子问题是否被重复求解？ | 用完整状态查缓存，复用已求出的结果 |

递归是一种实现方式，DFS 是探索顺序。DFS 也能用显式栈实现；递归也能实现快速幂等不以枚举方案为目标的算法。

板书用迷宫和决策树解释回溯。写代码时，把它落实成一条规则：**进入一个分支前修改了谁，离开后就检查谁是否需要恢复。**

注意区分三件事：

- **函数返回：**程序结束本次调用，回到调用者。
- **递归的“归”：**子问题计算完成，将控制权和结果交回上一层。
- **回溯恢复：**撤销选择对共享路径、访问标记等状态的影响。

`return` 不会自动撤销 `path.push_back(...)`；它也不会把通过引用修改的数组变回原样。只有本次调用的局部变量、按值参数等由调用栈分别保存。

## 3. 一道题的递归讨论顺序

### 3.1 第一步：先写“这一调用到底负责什么”

不要只写 `dfs(root)`，先用一句话定义任务：

```text
dfs(u) 负责求以 u 为根的整棵子树的高度，并把高度返回。
```

这句话同时确定了计算范围和返回值。然后才得到函数头：

```text
int dfs(TreeNode* u)
```

板书的“函数头”比“函数名”更完整：**返回类型 + 函数名 + 参数列表。**名字叫 `dfs`、`height` 或 `solve` 都可以；必须稳定的是它的含义。

把每个参数说到能区分“进入前”和“处理后”的程度：

```text
dfs(u, prefix)：prefix 是从根到 u 的父节点组成的数字，不包含 u。
dfs(u, remain)：remain 是包含 u 在内的剩余路径还需要凑出的和。
dfs(start)：下一次只允许从下标 start 及之后选择。
```

如果你无法说明参数是否包含当前节点，往往也无法准确写出更新语句和边界。

### 3.2 第二步：确定信息走哪个方向

```text
祖先积累的信息 ──参数──→ 当前节点 ──参数──→ 子节点
子树的计算结果 ←─返回值─ 当前节点 ←─返回值─ 子节点
```

| 需要什么信息？ | 常见设计 | 例子 |
|---|---|---|
| 祖先到当前节点的经历 | 参数向下传 | 路径和、祖先最大值、深度 |
| 当前节点以下的结果 | 返回值向上传 | 高度、子树和、剪枝后的根 |
| 已经构造的完整方案 | 存进答案容器 | 所有路径、所有排列 |
| 全局最优，但父节点只需要部分信息 | 返回局部信息，同时更新答案 | 直径、最大路径和 |

**自顶向下不等于没有返回值。**板书 07 的函数既接收 `presum`，又返回子树内所有完整数字的总和。

**有返回值不等于只能靠返回值记录最终答案。**直径题返回高度，同时更新直径。

### 3.3 第三步：把子调用当成已经能完成任务的工具

以高度为例：

```text
left  = dfs(u->left)   // 已经得到了左子树高度
right = dfs(u->right)  // 已经得到了右子树高度
本层只负责：max(left, right) + 1
```

这里的重点是板书强调的“只关心某一个子问题如何解决”。推导时先不用一路展开到最底层；验证时再展开一个小例子。

问自己：**如果左右子问题的答案已经写在纸上，本层还剩哪一行计算？**这通常就是递归关系。

### 3.4 第四步：出口与返回值一起讨论

| 子问题的含义 | 空节点常见返回值 | 为什么 |
|---|---|---|
| 高度、节点数、子树和 | `0` | 空树不贡献数量 |
| 所有节点都满足某条件 | `true` | 空树没有违反条件的节点 |
| 是否存在一条根到叶路径 | `false` | 空树不含这样的路径 |
| 处理后的子树根 | `nullptr` | 没有可交回的节点 |
| 收集路径的过程 | `return;` | 没有节点可继续收集 |

这不是可直接套用的返回值表。例如布尔表达式树中的空节点没有题目定义的真假值，就不要随便补一个 `false`。

什么时候必须特别处理叶子？

- 题目要求的是**根到叶子**的完整路径时，在叶子结算：129、112、113、257。
- 叶子本身就是天然答案时：2331 的叶子存储真假值。
- 只是在合并子树高度时：104 通常只判断空节点就够了。

必须区分：

```text
空节点：u == nullptr
叶子：  u != nullptr && !u->left && !u->right
```

### 3.5 第五步：检查状态、终止和正确性

**共享状态：**本层是否改了 `path`、`used`、计数器、上一个访问值？哪些修改属于当前路径，哪些属于整个遍历？

**终止：**每次调用是否更接近出口？二叉树转到孩子，排列剩余位置减少，子集下标增加。组合总和虽然可能继续使用同一下标，但在候选数均为正的前提下，剩余和严格减小。

**正确性：**边界正确；假设子调用按定义正确；检查本层组合是否得到自己承诺的结果。枚举题还要检查是否漏解、是否重复。

### 3.6 一张完整的函数讨论卡

```text
题目：
1. 相同子问题是什么：
2. 函数头：返回类型 函数名(参数)
3. 参数含义：进入时，是否已经包含当前节点？
4. 返回值含义：返回的是子树答案、可延伸信息，还是成功标记？
5. 本层函数体：准备什么 → 调哪些子问题 → 怎样合并/记录？
6. 递归出口：空节点？叶子？下标到末尾？方案已完成？
7. 共享状态：改变谁、何时恢复、谁不恢复？
8. 初始调用：参数初值？成员变量是否重置？
9. 终止依据与复杂度：
10. 用最小例子画图验证：
```

## 4. 决策树、调用树与返回过程怎么画

### 4.1 先分清画的是什么

| 图 | 一个节点表示什么 | 一条边表示什么 |
|---|---|---|
| 输入二叉树 | 题目给出的实际节点 | 实际父子关系 |
| 递归调用树 | 一次具体的函数调用及其状态 | 本次调用发起了一次子调用 |
| 搜索决策树 | 一种已经作出部分选择的状态 | 作出下一项选择 |

二叉树 DFS 的调用树经常与输入树相似，但调用状态还可能包含 `prefix`、`remain`、`path`。全排列没有输入二叉树，也能画出搜索决策树。

不要因为输入是一棵二叉树，就把代码中的所有 `if` 都画成搜索分支；也不要把“返回高度”误画成“又选择了一次”。

### 4.2 画图的七个步骤

1. 选一个足够小但有分叉的输入，通常 3—5 个节点或 2—3 个元素。
2. 在根框中写**函数名 + 实际参数 + 必要共享状态**。
3. 写明“一层处理什么”：一个树节点、一个位置，还是一个数组元素。
4. 按代码顺序画子调用，在边上写参数变化或选择内容。
5. 标出空节点、成功叶子、失败分支、剪掉的分支。
6. 子调用结束后，往回画箭头，并写上**返回值**。
7. 如果使用共享状态，在回边上标出 `pop_back()`、取消标记等恢复动作。

推荐标记：`✓` 表示记录答案，`×` 表示无效或剪枝，`↩` 表示返回，`恢复` 表示撤销共享修改。

### 4.3 例一：高度题——参数向下，数字向上

输入：

```text
       1
      / \
     2   3
```

调用树（`H(u)` 表示子树高度）：

```text
H(1) = max(1, 1) + 1 = 2
├── 调用 H(2) → 返回 1
│   ├── H(null) → 0
│   └── H(null) → 0
└── 调用 H(3) → 返回 1
    ├── H(null) → 0
    └── H(null) → 0
```

真正执行顺序是左分支完成后，才执行右分支：

```text
进入 H(1)
  进入 H(2)
    左空返回 0；右空返回 0
  H(2) 返回 1，H(1) 把它存进 left
  进入 H(3)
    左空返回 0；右空返回 0
  H(3) 返回 1，H(1) 把它存进 right
H(1) 返回 2
```

左右子调用画在同一层，表示它们的调用深度相同；不表示它们同时执行。

### 4.4 例二：路径题——回到父调用不等于路径已恢复

输入：`1` 的左孩子是叶子 `2`，右孩子是叶子 `3`。函数进入时 `path` 不包含当前节点，本层统一加入、退出前统一弹出：

```text
dfs(1)，进入时 []
  push 1 → [1]
  ├── dfs(2)，进入时 [1]
  │     push 2 → [1,2]
  │     ✓ 保存一份 [1,2]
  │     pop 2  → [1]
  │     ↩ 返回
  └── dfs(3)，进入时必须还是 [1]
        push 3 → [1,3]
        ✓ 保存一份 [1,3]
        pop 3  → [1]
        ↩ 返回
  pop 1 → []
  ↩ 返回
```

每个调用结束后满足一个不变式：**共享路径与进入该调用时完全一致。**

### 4.5 例三：排列题——一层填一个位置

输入 `[1,2,3]`，节点内写已经选择的前缀，边写本次选择。只从未使用的下标中选择：

```text
[]                         第 0 个位置
├── 选1 → [1]              第 1 个位置
│   ├── 选2 → [1,2]
│   │   └── 选3 → [1,2,3] ✓
│   └── 选3 → [1,3]
│       └── 选2 → [1,3,2] ✓
├── 选2 → [2]
│   ├── 选1 → [2,1] → 选3 → [2,1,3] ✓
│   └── 选3 → [2,3] → 选1 → [2,3,1] ✓
└── 选3 → [3]
    ├── 选1 → [3,1] → 选2 → [3,1,2] ✓
    └── 选2 → [3,2] → 选1 → [3,2,1] ✓
```

这里一层是一个位置；一条完整根到叶路径是一种排列。递归的孩子由循环生成，不是只有左、右两个孩子。

### 4.6 从图翻译成代码

| 图上的东西 | 代码里通常是什么 |
|---|---|
| 节点中随递归变化的信息 | 参数，或共享路径/标记 |
| 一个节点的多条出边 | 两次递归调用，或 `for` 循环 |
| 沿边作出选择 | 更新参数、加入路径、设置标记 |
| 回到父状态 | 返回后弹出路径、取消标记 |
| 完整方案 | 保存答案的条件 |
| 不需要继续的枝条 | `return`、`continue`，或有顺序依据的 `break` |

板书说要把决策树转化为代码。具体就是先写清这六行对应关系，再写语法。

## 5. 板书六题：逐题建立函数契约

### 5.1 板书 06：2331. 计算布尔二叉树的值

[题目](https://leetcode.cn/problems/evaluate-boolean-binary-tree/)

**相同子问题：**整棵表达式树能求值，左右表达式子树也能求值。

| 讨论项 | 本题答案 |
|---|---|
| 函数头 | `bool evaluateTree(TreeNode* root)` |
| 参数 | 当前表达式子树的根 |
| 返回值 | 这棵表达式子树的布尔结果 |
| 函数体 | 求左结果、右结果，再按当前运算符组合 |
| 出口 | 叶子直接返回存储的真假值 |
| 初始调用 | `evaluateTree(root)`；题目保证输入是合法的非空表达式树 |

<!-- example: B06 -->
```cpp
class Solution {
public:
    bool evaluateTree(TreeNode* root) {
        if (!root->left && !root->right)
            return root->val == 1;

        bool left = evaluateTree(root->left);
        bool right = evaluateTree(root->right);
        if (root->val == 2) return left || right;
        return left && right;  // 内部节点的另一个运算符为 3
    }
};
```

手画：根 `2`，左叶 `1`，右孩子为 `3` 且有叶子 `0、1`。

```text
evaluate(2) → true OR false → true
├── evaluate(1) → true
└── evaluate(3) → false AND true → false
    ├── evaluate(0) → false
    └── evaluate(1) → true
```

**为什么是后序思路？**当前运算符必须先拿到孩子结果才能计算。本例显式计算两边，访问每个节点一次，时间 `O(n)`，栈空间 `O(h)`。

如果直接写 `dfs(left) || dfs(right)`，C++ 的短路规则可能跳过右边。纯求值允许这样做；涉及路径清理、统计等副作用时，需要单独检查是否仍正确。

### 5.2 板书 07：129. 求根节点到叶节点数字之和

[题目](https://leetcode.cn/problems/sum-root-to-leaf-numbers/)

**相同子问题：**给定到父节点为止的前缀数字，求当前子树中所有根到叶完整数字的总和。

| 讨论项 | 本题答案 |
|---|---|
| 函数头 | `int dfs(TreeNode* u, int prefix)` |
| 参数 | `prefix` 是根到父节点组成的数字，不包含 `u` |
| 返回值 | 以该前缀进入当前子树后，所有完整数字的和 |
| 函数体 | 算 `current = prefix * 10 + u->val`，分别传给左右孩子，再把返回值相加 |
| 出口 | 空节点返回 0；叶子返回 `current` |
| 初始调用 | `dfs(root, 0)` |

<!-- example: B07 -->
```cpp
class Solution {
    int dfs(TreeNode* u, int prefix) {
        if (!u) return 0;
        int current = prefix * 10 + u->val;
        if (!u->left && !u->right) return current;
        return dfs(u->left, current) + dfs(u->right, current);
    }
public:
    int sumNumbers(TreeNode* root) {
        return dfs(root, 0);
    }
};
```

输入 `[1,2,3]`：

```text
dfs(1, 0)：current = 1；返回 12 + 13 = 25
├── dfs(2, 1)：current = 12；叶子 → 返回 12
└── dfs(3, 1)：current = 13；叶子 → 返回 13
```

**为什么不需要 `prefix /= 10`？**`prefix`、`current` 都是当前调用自己的整数；两个孩子分别拿到 `current` 的值，不共享可变的整数槽位。

**为什么不能只在空节点结算数字？**叶子有两个空孩子，这样容易把同一个完整数字算两次。

时间 `O(n)`，栈空间 `O(h)`。本代码按原题数值范围使用 `int`；迁移到更大范围时，需要同时检查中间数字与最终总和。

### 5.3 板书 08：814. 二叉树剪枝

[题目](https://leetcode.cn/problems/binary-tree-pruning/)

板书的关键是先处理左右子树，然后判断当前节点。一个原本有孩子的 0 节点，可能在孩子被剪掉后变成应被剪掉的叶子。

| 讨论项 | 本题答案 |
|---|---|
| 函数头 | `TreeNode* pruneTree(TreeNode* root)` |
| 参数 | 尚待处理的子树根 |
| 返回值 | 剪枝后这棵子树的新根，可能为 `nullptr` |
| 函数体 | 接回处理后的左右子树；若自己是无孩子的 0 节点，返回空 |
| 出口 | 空节点返回 `nullptr` |
| 初始调用 | `root = pruneTree(root)` |

<!-- example: B08 -->
```cpp
class Solution {
public:
    TreeNode* pruneTree(TreeNode* root) {
        if (!root) return nullptr;
        root->left = pruneTree(root->left);
        root->right = pruneTree(root->right);
        if (!root->left && !root->right && root->val == 0)
            return nullptr;
        return root;
    }
};
```

图例：`1` 左边接一条 `0 → 0` 的链，右边是叶子 `1`。

```text
prune(根1) → 根1
├── prune(中间0)
│   └── prune(叶子0) → null
│       中间0接回空孩子后，也返回 null
└── prune(叶子1) → 原来的叶子1

最终根1的左指针变成 null，右指针仍指向叶子1。
```

**为什么必须接收返回值？**`pruneTree(root->left);` 只调用函数，父节点的 `left` 并不会因为子调用返回了空而自动改变。

这里的“剪枝”是题目要求删除输入树中的子树，和搜索中跳过无效分支是两个语境。代码通过断开指针表达删树结果，没有执行 `delete`；独立工程中还需要按节点所有权处理内存释放。

时间 `O(n)`，栈空间 `O(h)`。

### 5.4 板书 09：98. 验证二叉搜索树

[题目](https://leetcode.cn/problems/validate-binary-search-tree/)

沿用板书的中序思路：合法 BST 的中序序列必须**严格递增**。仅比较当前节点与直接孩子不够，因为后代也受祖先约束。

| 讨论项 | 本题答案 |
|---|---|
| 函数头 | `bool dfs(TreeNode* u)` |
| 参数 | 当前待验证的子树根 |
| 外部状态 | `prev` 指向中序遍历中上一个已访问节点 |
| 返回值 | 从当前状态出发，处理该子树是否保持严格递增 |
| 函数体 | 左子树合法 → 比较当前值与 `prev` → 更新 `prev` → 验证右子树 |
| 出口 | 空树返回 `true`；发现逆序或相等返回 `false` |

注意这个 `dfs(u)` 的结果依赖外部的 `prev`，不能解释为“完全独立于遍历历史的子树属性”。

<!-- example: B09 -->
```cpp
class Solution {
    TreeNode* prev = nullptr;
    bool dfs(TreeNode* u) {
        if (!u) return true;
        if (!dfs(u->left)) return false;
        if (prev && prev->val >= u->val) return false;
        prev = u;
        return dfs(u->right);
    }
public:
    bool isValidBST(TreeNode* root) {
        prev = nullptr;
        return dfs(root);
    }
};
```

反例：

```text
       5
      / \
     1   7
        / \
       4   8

中序：1 → 5 → 4 → 7 → 8
访问4时，prev为5，5 >= 4，立即失败。
```

**为什么 `prev` 不恢复？**它记录的是整个中序序列的历史，不是当前根到叶路径。恢复它会破坏跨子树的比较。

最坏时间 `O(n)`，栈空间 `O(h)`。使用指针避免将 `INT_MIN` 作为哨兵时与真实节点值冲突。

补充迁移：也可以设计 `bool dfs(u, low, high)`，表示节点值必须落在开区间内；向左传 `(low, u->val)`，向右传 `(u->val, high)`。这就是把祖先限制作为参数向下传，边界类型应能覆盖整个输入范围。

### 5.5 板书 10：230. 二叉搜索树中第 K 小的元素

[题目](https://leetcode.cn/problems/kth-smallest-element-in-a-bst/)

板书使用中序遍历，加“剩余个数”和“答案”两个共享变量。下面额外让辅助函数返回成功标记，使提前停止能逐层传回去。

| 讨论项 | 本题答案 |
|---|---|
| 函数头 | `bool dfs(TreeNode* u)` |
| 外部状态 | `remaining`：还要访问几个节点才能到目标；`answer`：找到的值 |
| 返回值 | 是否已找到目标；不是第 k 小的数值本身 |
| 函数体 | 搜左；本节点令剩余个数减一；若尚未找到再搜右 |
| 出口 | 空节点返回 `false`；找到目标返回 `true` |

<!-- example: B10 -->
```cpp
class Solution {
    int remaining = 0;
    int answer = 0;
    bool dfs(TreeNode* u) {
        if (!u) return false;
        if (dfs(u->left)) return true;
        if (--remaining == 0) {
            answer = u->val;
            return true;
        }
        return dfs(u->right);
    }
public:
    int kthSmallest(TreeNode* root, int k) {
        remaining = k;
        answer = 0;
        dfs(root);
        return answer;
    }
};
```

题目保证 `k` 合法。中序依次访问 `1、2、3、4`，若 `k=2`，访问 2 后就逐层返回 `true`，不再访问 3、4。

**讨论重点：**辅助函数的返回值可以是控制流程的信息，最终答案另存在 `answer` 中。

时间 `O(h+k)`，最坏 `O(n)`；栈空间 `O(h)`。这里的界已经包含了找到第一个节点前的向下走访。

### 5.6 板书 11：257. 二叉树的所有路径

[题目](https://leetcode.cn/problems/binary-tree-paths/)

沿用板书的路径与结果容器。为清楚展示恢复过程，使用共享 `vector<int> path`，统一由当前调用加入和弹出当前值。

| 讨论项 | 本题答案 |
|---|---|
| 函数头 | `void dfs(TreeNode* u)` |
| 隐含输入 | 进入时 `path` 为根到父节点的路径 |
| 返回值 | 无；生成的字符串加入 `answer` |
| 函数体 | 加入本节点；叶子转成字符串，否则搜左右；最后弹出本节点 |
| 出口 | 空节点直接返回；叶子记录结果后走到统一恢复语句 |
| 不变式 | 函数返回时 `path` 与进入时一致 |

<!-- example: B11 -->
```cpp
class Solution {
    vector<int> path;
    vector<string> answer;
    void dfs(TreeNode* u) {
        if (!u) return;
        path.push_back(u->val);
        if (!u->left && !u->right) {
            string s;
            for (int x : path) {
                if (!s.empty()) s += "->";
                s += to_string(x);
            }
            answer.push_back(s);
        } else {
            dfs(u->left);
            dfs(u->right);
        }
        path.pop_back();
    }
public:
    vector<string> binaryTreePaths(TreeNode* root) {
        path.clear();
        answer.clear();
        dfs(root);
        return answer;
    }
};
```

手画过程见 4.4。板书里的 `dfs(root, path)` 也可以把路径显式作为参数；如果 C++ 中传 `vector<int>&` 或 `string&`，依然是共享对象，需要恢复；如果按值传入，每个调用持有副本，可省去手动撤销，但会增加复制成本。

时间 `O(n+L)`，辅助空间 `O(h)`（另计输出空间 `O(L)`）。收集所有路径时，应把生成结果的成本算进去，不能一律说 `O(n)`。

## 6. 从板书迁移到二叉树题单

这一节将板书的函数讨论方法用到题单中的其他二叉树问题。先关注**子问题需要交回什么**，再选择遍历顺序。

### 6.1 104 / 111：最大深度和最小深度

[104 最大深度](https://leetcode.cn/problems/maximum-depth-of-binary-tree/description/)按根到最远叶子的节点数计量。

```text
函数头：int maxDepth(TreeNode* root)
返回值：以 root 为根的子树高度。
函数体：1 + max(左高度, 右高度)。
出口：空树返回 0。
```

<!-- example: D104 -->
```cpp
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        int left = maxDepth(root->left);
        int right = maxDepth(root->right);
        return 1 + max(left, right);
    }
};
```

[111 最小深度](https://leetcode.cn/problems/minimum-depth-of-binary-tree/)容易错误地改成 `1 + min(left, right)`。先画只有一个孩子的树：

```text
1
 \
  2

正确最小深度为 2。空的左分支不是一条到叶子的路径。
```

<!-- example: D111 -->
```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        if (!root->left) return 1 + minDepth(root->right);
        if (!root->right) return 1 + minDepth(root->left);
        return 1 + min(minDepth(root->left), minDepth(root->right));
    }
};
```

**迁移方法：**两题返回值都叫“深度”，但本层合并规则必须服从题目的“叶子”条件。两种代码最坏时间 `O(n)`，栈空间 `O(h)`。

### 6.2 112 / 113：同一路径状态，返回方式不同

[112 路径总和](https://leetcode.cn/problems/path-sum/)只问“有没有”。

```text
函数头：bool hasPathSum(u, remain)
参数：包含 u 在内的剩余路径，还需要凑出的和。
返回值：当前子树里是否存在满足要求的 u 到叶路径。
函数体：扣掉 u->val，左右子树只要有一个成功即可。
出口：空节点 false；叶子检查扣除后是否为 0。
```

<!-- example: D112 -->
```cpp
class Solution {
public:
    bool hasPathSum(TreeNode* root, int remain) {
        if (!root) return false;
        remain -= root->val;
        if (!root->left && !root->right) return remain == 0;
        return hasPathSum(root->left, remain)
            || hasPathSum(root->right, remain);
    }
};
```

[113 路径总和 II](https://leetcode.cn/problems/path-sum-ii/description/)要求收集所有满足目标和的**根到叶**路径。将板书 11 的路径容器与 112 的剩余和结合：

<!-- example: D113 -->
```cpp
class Solution {
    vector<int> path;
    vector<vector<int>> answer;
    void dfs(TreeNode* u, long long remain) {
        if (!u) return;
        path.push_back(u->val);
        remain -= u->val;
        if (!u->left && !u->right) {
            if (remain == 0) answer.push_back(path);
        } else {
            dfs(u->left, remain);
            dfs(u->right, remain);
        }
        path.pop_back();
    }
public:
    vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        path.clear();
        answer.clear();
        dfs(root, targetSum);
        return answer;
    }
};
```

**函数契约：**`dfs(u, remain)` 在进入时，`path` 到父节点为止，`remain` 尚未扣除当前节点；函数把当前子树内的合法完整路径加入答案，退出时恢复 `path`。无返回值，因为它负责枚举并记录所有方案。

画一个最小成功例子：输入 `[1,2,3]`，目标 3。

```text
dfs(1,3)，path=[]
  本层：path=[1]，remain=2
  ├── dfs(2,2)：path=[1,2]，remain=0 → 叶子 ✓，恢复为[1]
  └── dfs(3,2)：path=[1,3]，remain=-1 → 叶子 ×，恢复为[1]
  恢复为[]
```

**不要凭 `remain < 0` 剪枝。**原题允许负数，例如目标 1，路径 `2 → -1` 仍然成功。

112 最坏时间 `O(n)`；113 时间 `O(n+L)`；两者辅助栈/路径空间均为 `O(h)`，113 另计输出。

### 6.3 110：返回值不够时，把它设计得更有信息

[110 平衡二叉树](https://leetcode.cn/problems/balanced-binary-tree/)

如果每到一个节点都重新遍历计算左右高度，链状树可能花 `O(n²)`。一次后序遍历中同时完成计算与判断：

```text
函数头：int heightOrFail(u)
返回值：>=0 表示该子树平衡，并给出高度；-1 表示不平衡。
函数体：先取得左右结果；失败就传播失败；否则比较高度并交回本层高度。
出口：空树高度为 0。
```

<!-- example: D110 -->
```cpp
class Solution {
    int heightOrFail(TreeNode* u) {
        if (!u) return 0;
        int left = heightOrFail(u->left);
        if (left == -1) return -1;
        int right = heightOrFail(u->right);
        if (right == -1) return -1;
        if (abs(left - right) > 1) return -1;
        return 1 + max(left, right);
    }
public:
    bool isBalanced(TreeNode* root) {
        return heightOrFail(root) != -1;
    }
};
```

`-1` 在这里安全，是因为合法高度不会为负数。若不存在安全哨兵，可返回 `pair<bool,int>` 等复合结果。

时间 `O(n)`，栈空间 `O(h)`。

### 6.4 543：递归返回值与题目答案可以不同

[543 二叉树的直径](https://leetcode.cn/problems/diameter-of-binary-tree/)按边数计量，最优路径不一定经过整棵树的根。

```text
函数头：int height(u)
返回值：当前子树高度，按节点数计算。
外部答案：直径的最大边数。
函数体：取得左右高度 L、R；用 L+R 更新答案；返回 1+max(L,R)。
出口：空树返回 0。
```

<!-- example: D543 -->
```cpp
class Solution {
    int answer = 0;
    int height(TreeNode* u) {
        if (!u) return 0;
        int left = height(u->left);
        int right = height(u->right);
        answer = max(answer, left + right);
        return 1 + max(left, right);
    }
public:
    int diameterOfBinaryTree(TreeNode* root) {
        answer = 0;
        height(root);
        return answer;
    }
};
```

为什么 `L+R` 正好是边数？左子树高度 `L` 是左孩子往下的节点数；从当前节点连接到该分支最深节点，恰好有 `L` 条边。右侧同理。

```text
     u
    / \
   a   b

u处：L=1，R=1，候选直径=2条边。
u向父节点返回：高度2，不是直径2的“同一含义”。
```

**为什么不返回 `L+R`？**父节点要接的是一条能继续向上延伸的链。若把左右都接好的折线路径交回父节点，再连父节点，就会形成分叉。

每条路径都有唯一的最高节点。遍历每个节点并计算经过它的候选路径，就不会漏掉最优直径。时间 `O(n)`，栈空间 `O(h)`。

### 6.5 124：将直径的“长度”换成“权值和”

[124 二叉树中的最大路径和](https://leetcode.cn/problems/binary-tree-maximum-path-sum/)要求路径至少包含一个节点。

```text
函数头：int gain(u)
返回值：以 u 为起点、向下延伸且不分叉的最大路径和，必须包含 u。
函数体：负的孩子贡献可舍弃；用 u.val+L+R 更新全局答案；
        向父节点只交回 u.val+max(L,R)。
出口：空节点贡献 0。
```

<!-- example: D124 -->
```cpp
class Solution {
    int answer = INT_MIN;
    int gain(TreeNode* u) {
        if (!u) return 0;
        int left = max(0, gain(u->left));
        int right = max(0, gain(u->right));
        answer = max(answer, u->val + left + right);
        return u->val + max(left, right);
    }
public:
    int maxPathSum(TreeNode* root) {
        answer = INT_MIN;
        gain(root);  // 原题保证至少有一个节点
        return answer;
    }
};
```

手算原题示例 `[-10,9,20,null,null,15,7]`：节点 20 用 `20+15+7=42` 更新答案，向父节点只返回 `20+15=35`。根节点候选为 `-10+9+35=34`，所以最终仍是 42。

**全负数检查：**单节点 `-3` 应返回 `-3`。若答案初始为 0，会错误地选到题目不允许的空路径。

时间 `O(n)`，栈空间 `O(h)`。本例的 `int` 适用原题数值范围。

### 6.6 236：返回一个节点，也是在返回子问题信息

[236 二叉树的最近公共祖先](https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/)保证 `p`、`q` 不同且都在树中。

```text
函数头：TreeNode* lowestCommonAncestor(u, p, q)
返回值：
  子树内都没有目标 → null；
  只有一个目标     → 返回该目标；
  两个目标都在     → 返回它们的最近公共祖先。
函数体：左右都有非空结果 → 当前节点；否则返回非空的一边。
出口：空节点；当前节点就是 p 或 q。
```

<!-- example: D236 -->
```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root,
                                   TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        return left ? left : right;
    }
};
```

手画 `3` 的左右孩子为 `5、1`，目标正好为 `5、1`：

```text
LCA(3)
├── LCA(5) → 返回目标5
└── LCA(1) → 返回目标1
左右均非空 → 返回3
```

若 `p` 是 `q` 的祖先，命中 `p` 直接返回仍然正确：在 `q` 确实存在的前提下，最近公共祖先就是 `p`。

如果题目改成目标可能不存在，不能直接照搬结论；需要额外统计实际找到了几个目标，或先验证存在性。时间 `O(n)`，栈空间 `O(h)`。

### 6.7 437：前缀和也有“恢复现场”

[437 路径总和 III](https://leetcode.cn/problems/path-sum-iii/)允许从任意节点开始、在任意后代结束，但路径方向必须向下。因此不能只在叶子结算，也不能只检查从根开始的和。

```text
函数头：void dfs(u, prefix)
参数：prefix 为根到父节点的和。
外部状态：freq[s] 为当前根路径上，祖先前缀和 s 的出现次数。
函数体：计算 current；累加 freq[current-target]；
        加入 current；搜左右；删除本次加入的 current。
出口：空节点直接返回。
初始状态：freq[0]=1，表示根之前的空前缀。
```

<!-- example: D437 -->
```cpp
class Solution {
    unordered_map<long long, int> freq;
    long long target = 0;
    int answer = 0;
    void dfs(TreeNode* u, long long prefix) {
        if (!u) return;
        long long current = prefix + u->val;
        auto it = freq.find(current - target);
        if (it != freq.end()) answer += it->second;
        ++freq[current];
        dfs(u->left, current);
        dfs(u->right, current);
        if (--freq[current] == 0) freq.erase(current);
    }
public:
    int pathSum(TreeNode* root, int targetSum) {
        freq.clear();
        freq[0] = 1;
        target = targetSum;
        answer = 0;
        dfs(root, 0);
        return answer;
    }
};
```

先查询、后加入当前前缀，是为了不把“当前前缀减自身”的空路径统计进去。

**为什么退出时必须减一？**左子树路径上的前缀不是右子树节点的祖先前缀。若保留，右子树会错误地与左子树配对。

手画防错例子 `[1,2,2]`，目标 0：正确为 0。若左叶子的前缀 3 未撤销，访问右叶子的相同前缀 3 时，就会错误统计一条跨兄弟分支的路径。

使用 `long long` 保存前缀和，避免多个节点相加溢出。哈希操作平均 `O(1)` 时，期望时间 `O(n)`，辅助空间 `O(h)`；哈希退化时不保证线性时间。

### 6.8 多个返回量：树形 DP、建树与双树比较

下面是进一步练习时应先写出的函数定义，暂不增加完整代码：

| 场景与练习 | 推荐函数含义 | 本层做什么 |
|---|---|---|
| [100 相同的树](https://leetcode.cn/problems/same-tree/) | `same(p,q)` 返回两棵子树是否相同 | 两空真、一空假；值相同且对应左右子树都相同 |
| [101 对称二叉树](https://leetcode.cn/problems/symmetric-tree/) | `mirror(p,q)` 返回两子树是否互为镜像 | 比较值；配对 `p.left/q.right`、`p.right/q.left` |
| [337 打家劫舍 III](https://leetcode.cn/problems/house-robber-iii/) | 返回 `(不选当前节点的最大收益, 选当前节点的最大收益)` | 不选可取孩子各自最优；选则孩子均不能选 |
| [105 从前序与中序构造二叉树](https://leetcode.cn/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) | `build(preL,preR,inL,inR)` 返回两个半开区间对应的子树根 | 前序定位根；中序定位左右范围；递归构造并连接 |
| [450 删除 BST 节点](https://leetcode.cn/problems/delete-node-in-a-bst/) | `erase(u,key)` 返回删除后的子树根 | 定位删除点；按孩子情况替换；父节点接回返回根 |

337 的递归关系可直接从含义写出。设左右孩子结果为 `(L0,L1)`、`(R0,R1)`：

```text
不选当前：skip = max(L0,L1) + max(R0,R1)
选择当前：take = u.val + L0 + R0
空树返回：(0,0)
最终答案：max(根.skip, 根.take)
```

两种状态是对方案集合的分类。这里求最大收益，不需要真的维护一条选择路径，也不需要写 `path.pop_back()`。

105 的函数出口是区间为空；区间必须约定清楚。若统一使用 `[L,R)`，长度就是 `R-L`，空区间就是 `L==R`。不能在父层用半开区间、子层又按闭区间计算。

## 7. 从二叉树走向排列、子集和组合

板书 12、13 的训练目标是：**题目没有给树，你自己把方案之间的关系画成树。**

### 7.1 46 全排列：一层填一个位置

[题目](https://leetcode.cn/problems/permutations/)

```text
函数头：void dfs()
隐含状态：path 是已填的前缀；used[i] 表示下标 i 是否已用于当前排列。
函数体：枚举所有未使用下标，选择 → 递归 → 撤销。
出口：path.size() == nums.size()，保存 path 的副本。
```

<!-- example: P46 -->
```cpp
class Solution {
    vector<int> path;
    vector<bool> used;
    vector<vector<int>> answer;
    void dfs(const vector<int>& nums) {
        if (path.size() == nums.size()) {
            answer.push_back(path);
            return;
        }
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfs(nums);
            path.pop_back();
            used[i] = false;
        }
    }
public:
    vector<vector<int>> permute(vector<int>& nums) {
        path.clear();
        answer.clear();
        used.assign(nums.size(), false);
        dfs(nums);
        return answer;
    }
};
```

决策树见 4.5。每个位置的可选下标数依次为 `n、n-1、…、1`，有 `n!` 个完整排列。考虑答案复制，时间 `O(n·n!)`，辅助空间 `O(n)`，输出空间 `O(n·n!)`。

**为什么按下标标记？**标记的是“哪个输入元素已经被使用”。出现相等数值时，下标仍然不同；如何避免相同方案，需要另外做同层去重。

### 7.2 78 子集：同一题可以画两种决策树

[题目](https://leetcode.cn/problems/subsets/)

板书 13 同时给出了“选/不选”和“下一次选谁”两种角度。

**画法一：一层决定一个元素是否加入。**输入 `[1,2]`：

```text
dfs(0)，path=[]
├── 不选1 → dfs(1)，[]
│   ├── 不选2 → dfs(2)，[]    ✓
│   └── 选2   → dfs(2)，[2]   ✓
└── 选1   → dfs(1)，[1]
    ├── 不选2 → dfs(2)，[1]   ✓
    └── 选2   → dfs(2)，[1,2] ✓
```

<!-- example: P78 -->
```cpp
class Solution {
    vector<int> path;
    vector<vector<int>> answer;
    void dfs(const vector<int>& nums, int i) {
        if (i == static_cast<int>(nums.size())) {
            answer.push_back(path);
            return;
        }
        dfs(nums, i + 1);           // 不选
        path.push_back(nums[i]);   // 选
        dfs(nums, i + 1);
        path.pop_back();
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        path.clear();
        answer.clear();
        dfs(nums, 0);
        return answer;
    }
};
```

**函数契约：**`dfs(nums,i)` 在前 `i` 个元素的取舍已经确定的条件下，枚举下标 `i` 及之后的所有取舍；进入与退出时共享 `path` 保持一致。`i==n` 才表示全部决定完成。

**画法二：一层决定下一个加入哪个元素。**每个节点的路径本身都是一个子集：

```text
[] ✓
├── 选下标0 → [1] ✓
│   └── 选下标1 → [1,2] ✓
└── 选下标1 → [2] ✓
```

对应的逻辑是：`dfs(start)` 先记录当前路径，再循环 `i=start..n-1`，选择 `nums[i]` 后调用 `dfs(i+1)`，回来撤销。

| 比较项 | 选/不选 | 枚举下一个元素 |
|---|---|---|
| 一层的含义 | 决定当前元素是否出现 | 确定下一个出现的元素 |
| 下标的含义 | 下一项要作决定的位置 | 下一次选择的最小下标 |
| 何时保存答案 | 所有元素都决定后 | 每次进入都保存 |
| 如何避免排列式重复 | 按固定顺序处理每个元素 | 下标只向后推进 |

两种画法都正确。不要拿第一种树配第二种的答案收集位置，否则可能漏解或重复。

两种方法考虑结果复制，时间 `O(n·2^n)`，辅助空间 `O(n)`，输出空间 `O(n·2^n)`。

### 7.3 47 全排列 II：同层去重与路径去重不同

[题目](https://leetcode.cn/problems/permutations-ii/)，对应板书 15。

先排序，在全排列循环中依次判断：

```text
if (used[i]) continue;
if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
```

第一句防止同一路径重复使用**同一个下标**。第二句要求相等元素按固定先后次序使用，避免同一层从相同数值出发，生成重复方案。

输入 `[1a,1b,2]`，`a、b` 只标明下标，数值都是 1：

```text
[]
├── 选1a → [1a]
│   └── 选1b → [1a,1b] ... 允许：上一相等元素已经在路径上
├── 选1b → ×               跳过：上一相等元素还未使用
└── 选2  → [2] ...
```

`used[i-1]` 的真假区分的是两种不同上下文。只写“遇到相同数字就跳过”，会把合法的 `[1,1,2]` 也删掉。

### 7.4 77 / 39：从“下一次允许选谁”设计参数

[77 组合](https://leetcode.cn/problems/combinations/)与[39 组合总和](https://leetcode.cn/problems/combination-sum/)对应板书 18、20。

| 题型 | 函数头的核心状态 | 选择下标 i 后 | 完成条件 |
|---|---|---|---|
| 子集 | `dfs(start)` | `dfs(i+1)` | 每个当前路径都可保存 |
| 恰选 k 个 | `dfs(start)`，并维护 `path` | `dfs(i+1)` | `path.size()==k` |
| 候选值可重复，凑目标和 | `dfs(start, remain)` | `dfs(i, remain-a[i])` | `remain==0` |
| 每个候选下标最多一次，凑目标和 | `dfs(start, remain)` | `dfs(i+1, remain-a[i])` | `remain==0` |

39 可以重复使用当前数，所以递归仍传 `i`；为避免把 `[2,3]` 和 `[3,2]` 当成两种组合，不允许再退回更小下标。

常见剪枝及证明：

- **组合剩余数量不足：**若还需选 `k-path.size()` 个，而可选元素更少，则无法完成。
- **正数候选超过剩余和：**排序后若 `a[i] > remain`，后面的数也都更大，可 `break`。若未排序，只能根据当前分支判断，不能直接断言后面也不行。
- **剩余和为负：**只有后续可选数都非负等保证不能补回来的条件下，才可据此判无解。39 的候选数为正，因此满足。

这就是板书“根据决策树写代码”的含义：参数 `i` 还是 `i+1`，由题目能否再次使用当前元素决定。

## 8. 网格搜索：哪些标记要恢复

### 8.1 把二叉树的两个孩子换成四个邻居

二叉树中，从父节点只走向孩子，通常不用额外标记。网格中两个相邻格子可以互相走到，若不限制，就可能来回递归。

板书 26 的四方向可以统一为：

```text
方向数组：(-1,0)、(1,0)、(0,-1)、(0,1)
新坐标：(x+dx[d], y+dy[d])
```

先问：**我是在寻找一条路径，还是要把一个连通块整体访问一遍？**

| 任务 | 标记的含义 | 返回后恢复吗？ |
|---|---|---|
| 79 单词搜索 | 当前这条候选路径已使用的格子 | 恢复，允许其他候选路径使用 |
| 200 岛屿数量 | 该陆地已经计入某个连通块 | 不恢复，防止重复计数 |
| 一般图的一次 DFS 遍历 | 这个顶点已被访问 | 通常不恢复 |
| 枚举简单路径 | 该顶点已在当前路径上 | 恢复，允许出现在其他路径里 |

### 8.2 79 单词搜索：返回真假，也要恢复路径标记

[题目](https://leetcode.cn/problems/word-search/)，对应板书 26。

```text
函数头：bool dfs(board,x,y,pos)
参数：尝试用格子(x,y)匹配 word[pos]，之前的字符已经匹配成功。
隐含状态：used 表示此前路径用过哪些格子。
返回值：能否从当前格子继续完成剩余匹配。
出口：越界、重复、字符不等 → false；匹配到最后字符 → true。
函数体：标记当前格子；试四方向；清除标记；返回是否成功。
```

<!-- example: G79 -->
```cpp
class Solution {
    string word;
    vector<vector<bool>> used;
    int rows = 0, cols = 0;
    bool dfs(const vector<vector<char>>& board, int x, int y, int pos) {
        if (x < 0 || x >= rows || y < 0 || y >= cols) return false;
        if (used[x][y] || board[x][y] != word[pos]) return false;
        if (pos + 1 == static_cast<int>(word.size())) return true;
        used[x][y] = true;
        bool found = dfs(board, x - 1, y, pos + 1)
                  || dfs(board, x + 1, y, pos + 1)
                  || dfs(board, x, y - 1, pos + 1)
                  || dfs(board, x, y + 1, pos + 1);
        used[x][y] = false;
        return found;
    }
public:
    bool exist(vector<vector<char>>& board, string target) {
        if (target.empty()) return true;
        if (board.empty() || board[0].empty()) return false;
        word = target;
        rows = static_cast<int>(board.size());
        cols = static_cast<int>(board[0].size());
        used.assign(rows, vector<bool>(cols, false));
        for (int x = 0; x < rows; ++x)
            for (int y = 0; y < cols; ++y)
                if (dfs(board, x, y, 0)) return true;
        return false;
    }
};
```

这里可以短路，因为找到一个成功方向就够了，而且本层在 `return found` 前统一取消了标记。最终字符的成功分支没有设置新标记，因此也没有遗漏恢复。

若改成“标记后直接 `return dfs(...) || ...`”，本层的恢复语句就可能被跳过。优先保持清楚的不变式：**辅助函数返回后，`used` 与进入时一致。**

设网格 `r×c`，单词长 `m`。宽松时间上界 `O(rc·4^m)`；标记数组 `O(rc)`，递归栈 `O(m)`。更紧的分析可考虑首步后不能立即原路返回。

### 8.3 200 岛屿数量：访问过的陆地不再恢复

[题目](https://leetcode.cn/problems/number-of-islands/)，对应板书 29、31。

```text
函数头：void flood(grid,x,y)
任务：把当前格子所在的、尚未处理的陆地连通块标记完。
返回值：无；通过修改网格留下“已访问”的结果。
出口：越界或不是陆地。
本层：标记当前陆地，然后搜索四邻居。
```

<!-- example: G200 -->
```cpp
class Solution {
    int rows = 0, cols = 0;
    void flood(vector<vector<char>>& grid, int x, int y) {
        if (x < 0 || x >= rows || y < 0 || y >= cols) return;
        if (grid[x][y] != '1') return;
        grid[x][y] = '0';
        flood(grid, x - 1, y);
        flood(grid, x + 1, y);
        flood(grid, x, y - 1);
        flood(grid, x, y + 1);
    }
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        rows = static_cast<int>(grid.size());
        cols = static_cast<int>(grid[0].size());
        int answer = 0;
        for (int x = 0; x < rows; ++x)
            for (int y = 0; y < cols; ++y)
                if (grid[x][y] == '1') {
                    ++answer;
                    flood(grid, x, y);
                }
        return answer;
    }
};
```

外层每遇到一个未处理的陆地，就发现了一个新连通块。FloodFill 负责将整个块处理完。**本实现会修改输入网格**；需要保留输入时改用独立访问数组。

时间 `O(rc)`，最坏递归栈 `O(rc)`。如果把 `'0'` 恢复为 `'1'`，便失去“已计入连通块”的标记，还可能重新进入已搜索格子。

## 9. 记忆化搜索：什么才算相同的子问题

### 9.1 从板书 37 的斐波那契开始

板书先画出递归展开，再标记重复的子问题：

```text
F(5)
├── F(4)
│   ├── F(3)
│   └── F(2)
└── F(3) ← 与左边计算的是完全相同的状态
```

函数契约：`fib(n)` 返回第 `n` 个斐波那契数；出口 `n<=1`；函数体为 `fib(n-1)+fib(n-2)`。这正对应“状态含义、状态转移、初始条件”。

<!-- example: M509 -->
```cpp
class Solution {
    vector<int> memo;
    int dfs(int n) {
        if (n <= 1) return n;
        if (memo[n] != -1) return memo[n];
        return memo[n] = dfs(n - 1) + dfs(n - 2);
    }
public:
    int fib(int n) {
        memo.assign(n + 1, -1);
        return dfs(n);
    }
};
```

[509 斐波那契数](https://leetcode.cn/problems/fibonacci-number/)原题范围内可使用 `int`。普通递归有指数级重复计算；记忆化后每个 `n` 最多求一次，时间 `O(n)`，缓存和栈空间 `O(n)`。

### 9.2 缓存键必须覆盖影响答案的完整状态

“同一个函数”不等于“同一个子问题”。必须在影响后续答案的信息都相同的条件下，才能复用结果。

| 问题 | 常见完整状态 | 容易遗漏什么 |
|---|---|---|
| 斐波那契 | `n` | 输入固定时，仅 n 即可 |
| 从某格到终点的路径数量 | `(x,y)`，且障碍等输入固定 | 若还有限制条件，可能要额外状态 |
| 目标和计数 | `(i,remain)` | 只缓存 i，忽略不同剩余和 |
| 单词搜索 | `(x,y,pos,已使用格子集合)` | 只缓存坐标和字符位置，忽略路径禁用格子 |
| 验证 BST 的上下界写法 | `(u,low,high)` | 同一子树在不同祖先约束下未必同结果 |

`path`、`used`、`prev` 等隐含状态也可能影响结果。把它们藏成成员变量，并不代表缓存键就能忽略它们。

### 9.3 板书 41：矩阵中的最长递增路径

[329 矩阵中的最长递增路径](https://leetcode.cn/problems/longest-increasing-path-in-a-matrix/)可以定义：

```text
dfs(x,y)：以(x,y)为起点的最长严格递增路径长度。
本层：答案初始为1；对值更大的四邻居，取 max(答案,1+dfs(nx,ny))。
出口：没有更大的邻居时，结果自然为1。
缓存键：(x,y)。
最终答案：对每个起点的返回值取最大值。
```

为什么这里不需要把访问路径放进缓存键？严格递增使路径无法成环；后续能走到哪里只由当前位置和固定矩阵决定，不依赖先前从哪里来。

每个格子只计算一次，每次检查四个方向，时间 `O(rc)`，缓存与最坏递归栈共 `O(rc)`。

不要把“记忆化搜索的自顶向下”与二叉树“祖先状态向下传”混为一谈：前者说的是从所求状态递归求依赖状态的计算方式；高度题即便在返回时合并，也仍可用递归方式完成。

## 10. 题型辨认与练习路线

以下路线结合压缩包课程与外部题单，按理解依赖重新安排；不是外部题单的完整重排。

### 10.1 先用问题选择函数设计

```text
我需要什么？
├── 子树独立产生一个结果
│   ├── 数字/真假：高度、表达式求值、平衡判断
│   ├── 节点指针：剪枝、删点、建树、最近公共祖先
│   └── 多个量：高度+标志、选/不选收益
├── 从祖先继承信息
│   └── 参数携带前缀和、剩余和、边界、祖先最值
├── 枚举具体方案
│   └── 路径/标记 + 选择 + 递归 + 恢复 + 答案容器
└── 返回给父节点的东西不是最终答案
    └── 返回可延伸的局部结果，同时更新最优答案
```

这是一组分析方向，可以同时成立。129 既继承前缀，也返回结果；437 既携带前缀，也恢复路径上的计数。

### 10.2 推荐分阶段练习

| 阶段 | 练习 | 做完后应能回答 |
|---|---|---|
| 1. 看懂一层 | 板书 01 汉诺塔；104；板书 06 / 2331 | 为什么子调用可以看成一个已经可用的工具？ |
| 2. 传参和返回 | 板书 07 / 129；112；111 | 参数是否包含当前节点？叶子与空节点分别做什么？ |
| 3. 返回结构 | 板书 08 / 814；226；1325 | 返回的指针为什么要接回父节点？ |
| 4. 中序与历史 | 板书 09 / 98；板书 10 / 230 | `prev`、计数器为什么不按路径恢复？ |
| 5. 显式回溯 | 板书 11 / 257；113；板书 12 / 46；板书 13 / 78 | 一层、一条边、一片叶子分别代表什么？ |
| 6. 信息升级 | 110；543；124；236 | 父节点需要的信息与题目答案有何区别？ |
| 7. 搜索优化 | 板书 15 / 47；板书 20 / 39；437 | 同层去重与路径标记的区别是什么？ |
| 8. 网格与缓存 | 板书 26 / 79；31 / 200；37 / 509；41 / 329 | 哪些状态应恢复？哪些结果可以缓存？ |
| 9. 进阶 | 337；105；450 | 如何返回多个量，或构造/修改结构？ |

其中 [226 翻转二叉树](https://leetcode.cn/problems/invert-binary-tree/)和[1325 删除给定值的叶子节点](https://leetcode.cn/problems/delete-leaves-with-a-given-value/)可作为板书 08 的相邻练习：都要先明确“处理后返回什么”。

BFS 可作为对照学习：[102 层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/)适合直接按层收集；111 可在 BFS 第一次遇到叶子时得到最小深度。不能仅因为题目是二叉树，就认定必须用递归。

### 10.3 每道题做三轮

1. **第一轮：讨论。**先写函数讨论卡与三层以内的小图，暂不看完整代码。
2. **第二轮：实现。**只看函数契约写代码，再用空树、单节点、单侧树验证。
3. **第三轮：迁移。**改一个条件：求有无改成收集全部、根到叶改成任意起点、返回数值改成返回节点，再检查哪些设计必须改变。

能解释“条件改了以后，为什么函数定义要跟着改”，比记住一段固定模板更接近真正掌握。

## 11. 常见错误与检查方法

| 常见错误 | 会出现什么问题 | 检查办法 |
|---|---|---|
| 参数含义模糊 | 当前节点被重复计入或漏算 | 明确进入时是否包含当前节点 |
| 所有题都让空节点返回 0 | 布尔判断、结构修改含义错误 | 从返回值语义推导出口 |
| 把空节点当叶子 | 单侧树深度错误、完整路径重复结算 | 用只有一个孩子的树手算 |
| 子调用返回新根却不接回 | 删点、剪枝、建树无效 | 检查 `left = dfs(left)` 等赋值 |
| 给父节点返回左右拼接路径 | 上层再连接后形成分叉 | 返回值只能保留父层可使用的形态 |
| 保存答案后直接提前返回 | `path`、`used` 未恢复 | 每条退出路径都核对不变式 |
| 所有共享变量都恢复 | 中序 `prev`、全局最优被破坏 | 区分遍历历史、全局答案和路径状态 |
| 每次进入节点都记录路径 | 把前缀误当成根到叶完整路径 | 检查题目要求的终点条件 |
| 同层遇相等值就无条件跳过 | 相同值不能合法连续出现 | 结合排序与上一元素的使用状态 |
| 没有条件证明就剪枝 | 负数路径、未排序候选被漏掉 | 写一句“此分支不可能成功”的证明 |
| 前缀和计数不撤销 | 左右分支被错误配对 | 计数表只包含当前祖先路径 |
| 缓存键少参数 | 在不同约束下复用错误结果 | 列出影响后续答案的所有显式/隐含状态 |
| 每个节点重新遍历子树 | 时间可能从线性退化到平方 | 看子调用能否顺便返回父层需要的信息 |
| 答案或计数器不重置 | 同一 Solution 对象第二次调用出错 | 在公开入口初始化成员变量 |

**复杂度检查：**二叉树上每个节点只做常数工作，常见时间为 `O(n)`；枚举所有子集或排列则必须考虑方案数量。递归空间看最大调用深度，不看整棵调用树的总节点数。

**深度检查：**树或网格可能退化成很长的链。递归实现受运行时栈大小限制；当输入很深时，可按相同状态设计改写为显式栈、队列或迭代 DP。

## 12. 可复制的讨论模板与自测

### 12.1 遇到一道新题，按这个顺序讲给自己听

```text
这道题的大问题是：____________________
它的同类小问题是：____________________

【函数名与函数头】
返回类型：________
函数名：________
参数列表：________
进入本次调用时，参数/共享状态分别表示：________

【返回值】
本次调用承诺交给父调用：________
它与题目最终答案是否相同：________

【函数体：只看一层】
1. 当前节点/当前选择先做：________
2. 下一层分别调用：________
3. 假设子调用已正确完成，本层如何组合或记录：________

【出口与状态】
空状态：________
成功状态：________
失败/剪枝条件及依据：________
修改了哪些共享变量：________
退出前恢复哪些变量，哪些不用恢复：________

【初始调用与验证】
入口参数和外部状态初值：________
递归为什么会结束：________
手画输入：________
图中的一层、一条边、一个出口分别代表：________
预计时间与空间：________
```

### 12.2 六个不用背代码的自测

1. **129 中，左分支走完后为什么右分支没有带上左分支的数字？**
   因为前缀整数按值传递，每个调用有自己的参数值。若改成共享可变容器，就需要重新讨论恢复。
2. **257 中，函数返回后 `path` 会自动少一个元素吗？**
   不会。当前实现使用共享容器，必须执行 `pop_back()`。
3. **98 中，左子树处理完为什么不恢复 `prev`？**
   因为下一个中序节点应该与整段已访问序列的最后节点比较。
4. **543 中，为什么答案用左右之和，返回值却只取较大的一边？**
   当前节点可作为一条完整路径的转折点，但交给父节点的路径必须还能延伸，不能分叉。
5. **113 中，剩余和已经小于 0，能否立即返回？**
   原题允许负数，不能仅凭这一条件剪枝。
6. **79 与 200 都访问网格，为什么一个恢复标记，一个保留标记？**
   前者标记当前候选路径用过的格子，后者标记已被计入连通块的格子，标记含义不同。

### 12.3 手算用例清单

| 题目/场景 | 输入 | 预期 |
|---|---|---|
| 104 | 空树 | 0 |
| 111 | 根1只有右孩子2 | 2 |
| 129 | `[1,2,3]` | 25 |
| 814 | `[0,0,0]` | 空树 |
| 98 | 根5，右孩子7，7的左孩子4 | false |
| 230 | BST `[3,1,4,null,2]`，k=2 | 2 |
| 257 | `[1,2,3]` | `1->2`、`1->3` |
| 112 / 113 | 根2只有左叶-1，目标1 | 存在；路径 `[2,-1]` |
| 110 | 根1的左侧是2→3，右侧为空 | false |
| 543 | `[1,2,3]` | 2条边 |
| 124 | 单节点-3 | -3 |
| 236 | 根3的孩子5、1，目标为两个孩子 | 根节点3 |
| 437 | `[1,2,2]`，目标0 | 0，防止跨兄弟分支计数 |
| 46 | `[1,2,3]` | 6个排列 |
| 78 | `[1,2]` | 4个子集 |
| 79 | 只有一个字符A，目标AA | false，不能重复使用格子 |
| 200 | 四个格子构成全陆地2×2网格 | 1个岛屿 |
| 509 | n=0 / n=1 / n=10 | 0 / 1 / 55 |

判断自己是否已经能独立做题，可以用一个标准：**看着函数头，能说清每个输入、返回值和共享状态；遮住代码，能画出下一层；沿图返回时，能说清结果如何合并、状态如何恢复。**
