for(int i = 1; i < n; ++i) {
    Solution::TreeNode* curr = q.front();
    q.pop();
    int left = tree[i++];
    int right = tree[i];
    if (left != 999) {
      curr->left = new Solution::TreeNode(left);
      q.push(curr->left);
    }
    if (right != 999) {
      curr->right = new Solution::TreeNode(right);
      q.push(curr->right);
    }
  }