/*
题目内容
云化平台对于组件按版本号进行统一管理，版本号遵循语义版本控制系统，格式为：

major.minor.patch

其中：
- major：项目进行重大更新，且包含不向后兼容的功能
- minor：仅在以向后兼容的方式添加新功能
- patch：仅在以向后兼容的方式修复错误，并且没有添加任何新的功能

业务 App 在其配置文件中声明其依赖的组件及版本，如：
1.1.0

并且版本号可以缩写，没有写出来的部分表示为 0，如：
1.2 等价于 1.2.0
1 等价于 1.0.0

对于其依赖的版本号，有三种声明方式：

1）指定最新版本
业务 App 的配置方式为：*
它最终会匹配仓库中的最新版本给业务 App 使用。

2）取 major 版本一致情况下的最大版本
由于 major 版本之间可能不兼容，指定主版本号，业务 App 的配置方式为：^1.2.0
这样写会尝试匹配主版本号为 1 的最大版本。

更多样例含义：
^1.2 等价于 ^1.2.0
^1 等价于 ^1.0.0

3）保证 major 和 minor 版本一致，只取最新的 patch 版本
只进行 Bug 修复，只更新 patch，业务 App 的配置方式为：~1.2.0 或者 -1.2.0
这样写会取 major 版本号为 1、minor 版本号为 2、patch 版本号最大的版本。

输入描述
第一行给出当前仓库中指定组件的版本数量 N，
后续 N 行给出具体的每一个版本号（注意这里不保证版本号的大小顺序），
最后一行给出业务 App 组件的版本号声明。

3 <= N <= 100000
版本号中的 major、minor 和 patch 都满足：
0 <= version <= 100000

输出描述
输出最终匹配到的版本号。
如果没有匹配到的版本号则输出 None。

样例1
输入：
5
1.1.1
2.1
1.2.2
1.2.6
1.3.3
^1.2

输出：
1.3.3

说明：
对于目标组件，当前在仓库中有 5 个版本，分别为：
1.1.1
2.1
1.2.2
1.2.6
1.3.3
业务 App 声明的组件依赖为 ^1.2，系统最终为其匹配到的版本为 1.3.3。

样例2
输入：
5
1.1.1
2.1
1.2.2
1.2.6
1.3.3
*

输出：
2.1

说明：
* 表示取所有版本中的最高版本，即 2.1。

样例3
输入：
5
1.1.1
2.1
1.2.2
1.2.6
1.3.3
-1.2

输出：
1.2.6

说明：
- 表示在 major.minor 不变的情况下取 patch 最大的版本号，这里即 1.2.6。

样例4
输入：
6
1.1.1
2.1
1.2.2
1.2.6
1.3.3
1.4
^1.6

输出：
None

说明：
major 版本号为 1 且 minor 大于等于 6 的版本号不存在，输出 None。
*/

#include <algorithm>
#include <bits/stdc++.h>
#include <string>
using namespace std;

class Solution {
public:
  struct Version {
    int major;
    int minor;
    int patch;

    Version(int m, int mi, int p) : major(m), minor(mi), patch(p) {}

    };

    Version Versionize(string& s) {
      Version v{0, 0, 0 };
      stringstream ss(s);
      string token;
      vector<int> tmp;
      while (getline(ss, token, '.')) {
        tmp.emplace_back(stoi(token));
      }
      int size = tmp.size();
      if (size > 0) v.major = tmp[0];
      if (size > 1) v.minor = tmp[1];
      if (size > 2) v.patch = tmp[2];
     
      return v;
    }

    static bool VersionCompare(const Version& a, const Version& b) {
      if (a.major != b.major) return a.major > b.major;
      if (a.minor != b.minor) return a.minor > b.minor;
      return a.patch > b.patch;
    }

    string VersionSelection(vector<string>& versions, string& select) {
      vector<Version> ob_versions;
      for (auto& s : versions) {
        ob_versions.emplace_back(Versionize(s));
      }
      sort(ob_versions.begin(), ob_versions.end(), VersionCompare);
      
      Version tmp = ob_versions[0];
      
      if (select[0] == '*') {
        return to_string(tmp.major) + "." + to_string(tmp.minor) + '.' + to_string(tmp.patch); 
      }

      
      int major_target = select[1] - '0';
      int minor_target = select.size() >= 4 ? select[3] - '0' : 0;
      int patch_target = select.size() >= 6 ? select[5] - '0' : 0;
      bool is_found = false;
      if (select[0] == '-') {
        for (int i = 0; i < ob_versions.size(); ++i) {
          if (ob_versions[i].major == major_target && ob_versions[i].minor == minor_target) {
            tmp = ob_versions[i];
            is_found = true;
            break;
          } 
        }
      }  

      for (int i = 0; i < ob_versions.size(); ++i) {
        if (major_target == ob_versions[i].major && minor_target <= ob_versions[i].minor) {
          tmp = ob_versions[i];
          is_found = true;
          break;
        }
      }
      if (is_found) return to_string(tmp.major) + "." + to_string(tmp.minor) + '.' + to_string(tmp.patch);
      return "None";
    }

};

int main() {
  int n;
  cin >> n;
  vector<string> versions(n);
  for (int i = 0; i < n; ++i) {
    cin >> versions[i];
  }

  string select;
  cin >> select;

  Solution sol;
  string res = sol.VersionSelection(versions, select);
  cout << res << endl;
}