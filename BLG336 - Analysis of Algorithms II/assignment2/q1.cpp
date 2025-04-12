// https://www.hackerrank.com/challenges/ctci-connected-cell-in-a-grid/problem

#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'maxRegion' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts 2D_INTEGER_ARRAY grid as parameter.
 */

int maxRegion(vector<vector<int>> grid) {
    int n = grid.size();
    int m = grid[0].size();
    
    vector<vector<bool>> vis(n, vector<bool>(m, false));
    vector<pair<int,int>> directions = {
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}, 
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1}
    };

    queue<pair<int, int>> q;

    int max_reg = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(!vis[i][j] && grid[i][j] == 1){
                int cur_reg = 1;
                q.push({i, j});
                vis[i][j] = true;

                while(!q.empty()){
                    int x = q.front().first;
                    int y = q.front().second;
                    q.pop();

                    for(auto [dx, dy] : directions){
                        int x_n = x + dx;
                        int y_n = y + dy;
                        if(0 <= x_n && x_n < n && 0 <= y_n && y_n < m && !vis[x_n][y_n] && grid[x_n][y_n] == 1){
                            q.push({x_n, y_n});
                            vis[x_n][y_n] = true;
                            cur_reg++;
                        }
                    }
                }
                max_reg = max(max_reg, cur_reg);
            }
        }
    }
    return max_reg;

}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string m_temp;
    getline(cin, m_temp);

    int m = stoi(ltrim(rtrim(m_temp)));

    vector<vector<int>> grid(n);

    for (int i = 0; i < n; i++) {
        grid[i].resize(m);

        string grid_row_temp_temp;
        getline(cin, grid_row_temp_temp);

        vector<string> grid_row_temp = split(rtrim(grid_row_temp_temp));

        for (int j = 0; j < m; j++) {
            int grid_row_item = stoi(grid_row_temp[j]);

            grid[i][j] = grid_row_item;
        }
    }

    int res = maxRegion(grid);

    fout << res << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
