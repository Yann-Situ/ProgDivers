#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

using namespace std;

int temps(int c, int c1, int t, vector<bool> &feux_rouge, vector<bool> &mur)
{
  if (mur[c] || mur[c1])
    return 90;
  if (feux_rouge[c])
    return 2;
  if ((t%2 == 1) && feux_rouge[c1])
    return 2;
  return 1;
}
int min(int a, int b)
{
  if (a < b)
    return a;
  return b;
}
int dijkstra(vector<bool> &feux_rouge, vector<bool> &mur, int debut, int objectif, int m, int T)
{
  vector<int> distance(feux_rouge.size(), 90);
  distance[debut] = T;
  vector<bool> vu(feux_rouge.size(), false);
  int compte_vu = 0;
  while (compte_vu < feux_rouge.size()) {
    compte_vu++;
    int a = -99999999;
    int distance_mini = 200;
    for (int i = 0; i < distance.size(); i++) {
      if ((distance[i] < distance_mini) && (vu[i] == false)) {
        a = i;
        distance_mini = distance[i];
      }
    }
    if (distance_mini > 150)
      break;
    vu[a] = true;
    if (a%m != m-1)
      distance[a+1] = min(distance[a+1], distance[a] + temps(a, a+1, distance[a], feux_rouge, mur));
    if (a%m != 0)
      distance[a-1] = min(distance[a-1], distance[a] + temps(a, a-1, distance[a], feux_rouge, mur));
    if (a+m < distance.size())
      distance[a+m] = min(distance[a+m], distance[a] + temps(a, a+m, distance[a], feux_rouge, mur));
    if (a-m >= 0)
      distance[a-m] = min(distance[a-m], distance[a] + temps(a, a-m, distance[a], feux_rouge, mur));
  }
  //for (int i = 0; i < m; i++) {
  //  for (int j = 0; j < m; j++) {
        //cout << i << " " << j << " : " << distance[i*m+j]-T << endl;
  //  }
  //}
  return distance[objectif]-T;
}
int main()
{
  int n, t;
  cin >> n >> t;
  vector<string> noms;
  vector<vector<pair<int, int>>> positions;
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;
    noms.push_back(s);
    positions.emplace_back(0);
    for (int j = 0; j < t; j++) {
      int a, b;
      cin >> a >> b;
      a--; b--;
      pair<int, int>tmp(a, b);
      positions[i].push_back(tmp);
    }
  }
  //vector<vector<int>> graphe;
  int m;
  cin >> m;
  vector<bool> feux_rouge(m*m, false);
  vector<bool> mur(m*m, false);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      int c;
      cin >> c;
      if (c == 2)
        feux_rouge[i*m+j] = true;
      if (c == 1)
        mur[i*m+j] = true;
    }
  }

  int bombex, bombey;
  cin >> bombex >> bombey;
  bombex--;
  bombey--;
  //cout << "Bombe : " << bombex << " " << bombey << endl;
  vector<string> suspects;
  for (int i = 0; i < n; i++) {
    //cout << i << endl;
    bool ok = false;
    for (int j = 0; j < t-1; j++) {
      //cout << noms[i] << endl;
      //cout << "Position : " << positions[i][j].first << " " << positions[i][j].second << endl;
      if (ok == false) {
        int temps1 = dijkstra(feux_rouge, mur, positions[i][j].first*m+positions[i][j].second,bombex*m+bombey, m, 0);
        int temps2 = dijkstra(feux_rouge, mur, bombex*m+bombey, positions[i][j+1].first*m+positions[i][j+1].second, m, (temps1+1)%2);
        if (temps1 + temps2 < 60) {
          suspects.push_back(noms[i]);
          ok = true;
        }
      }
    }
  }
  sort(suspects.begin(), suspects.end());
  if (suspects.empty())
    cout << "NONE" << endl;
  else {
    for (string &s: suspects)
      cout << s << " ";
  }
}
