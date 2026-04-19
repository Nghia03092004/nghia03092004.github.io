#include <bits/stdc++.h>
using namespace std;

// IOI 2020 - Connecting Supertrees
// Build a graph where the number of simple paths between i and j equals p[i][j].
// p[i][j]=0: different components. p=1: tree. p=2: cycle. p=3: cycle of cycles.

struct DSU {
    vector<int> par, rnk;
    DSU(int n) : par(n), rnk(n, 0) { iota(par.begin(), par.end(), 0); }
    int find(int x) { return par[x] == x ? x : par[x] = find(par[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) swap(a, b);
        par[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        return true;
    }
};

int construct(vector<vector<int>> p) {
    int n = (int)p.size();
    vector<vector<int>> answer(n, vector<int>(n, 0));

    // Validate diagonal and symmetry
    for (int i = 0; i < n; i++) {
        if (p[i][i] != 1) return 0;
        for (int j = 0; j < n; j++)
            if (p[i][j] != p[j][i]) return 0;
    }

    // Step 1: Group by p > 0 (connected components)
    DSU comp(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i][j] > 0) comp.unite(i, j);

    // Verify component consistency
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            if (comp.find(i) == comp.find(j) && p[i][j] == 0) return 0;
            if (comp.find(i) != comp.find(j) && p[i][j] != 0) return 0;
        }

    // Collect components
    map<int, vector<int>> components;
    for (int i = 0; i < n; i++)
        components[comp.find(i)].push_back(i);

    for (auto& [root, nodes] : components) {
        int sz = (int)nodes.size();
        if (sz == 1) continue;

        // Step 2: Within component, group by p = 1 (tree sub-groups)
        DSU tree_group(n);
        for (int a : nodes)
            for (int b : nodes)
                if (p[a][b] == 1) tree_group.unite(a, b);

        // Verify tree group consistency
        for (int a : nodes)
            for (int b : nodes)
                if (tree_group.find(a) == tree_group.find(b) && p[a][b] != 1)
                    return 0;

        // Collect tree groups and connect each as a chain
        map<int, vector<int>> tgroups;
        for (int a : nodes)
            tgroups[tree_group.find(a)].push_back(a);

        for (auto& [tr, tnodes] : tgroups) {
            for (int i = 0; i + 1 < (int)tnodes.size(); i++) {
                answer[tnodes[i]][tnodes[i + 1]] = 1;
                answer[tnodes[i + 1]][tnodes[i]] = 1;
            }
        }

        // Step 3: Handle inter-group connections
        vector<vector<int>> group_list;
        map<int, int> group_id;
        for (auto& [tr, tnodes] : tgroups) {
            group_id[tr] = (int)group_list.size();
            group_list.push_back(tnodes);
        }
        int ng = (int)group_list.size();
        if (ng == 1) continue;

        // Check consistency between tree groups
        vector<vector<int>> inter_p(ng, vector<int>(ng, 0));
        for (int gi = 0; gi < ng; gi++)
            for (int gj = gi + 1; gj < ng; gj++) {
                int val = p[group_list[gi][0]][group_list[gj][0]];
                for (int a : group_list[gi])
                    for (int b : group_list[gj])
                        if (p[a][b] != val) return 0;
                inter_p[gi][gj] = inter_p[gj][gi] = val;
            }

        // Determine structure type
        bool all_two = true, has_three = false;
        for (int gi = 0; gi < ng; gi++)
            for (int gj = gi + 1; gj < ng; gj++) {
                if (inter_p[gi][gj] != 2) all_two = false;
                if (inter_p[gi][gj] == 3) has_three = true;
            }

        if (all_two) {
            // Connect groups in a single cycle (need >= 3 total nodes)
            if (ng < 3) return 0;
            for (int gi = 0; gi < ng; gi++) {
                int gj = (gi + 1) % ng;
                int a = group_list[gi][0];
                int b = group_list[gj][0];
                answer[a][b] = answer[b][a] = 1;
            }
        } else if (has_three) {
            // Group tree-groups into "2-groups" by p=2, then connect 2-groups in cycle for p=3
            DSU cycle_group(ng);
            for (int gi = 0; gi < ng; gi++)
                for (int gj = gi + 1; gj < ng; gj++)
                    if (inter_p[gi][gj] == 2) cycle_group.unite(gi, gj);

            // Verify 2-group consistency
            for (int gi = 0; gi < ng; gi++)
                for (int gj = gi + 1; gj < ng; gj++) {
                    if (cycle_group.find(gi) == cycle_group.find(gj)
                        && inter_p[gi][gj] != 2) return 0;
                    if (cycle_group.find(gi) != cycle_group.find(gj)
                        && inter_p[gi][gj] != 3) return 0;
                }

            // Collect 2-groups
            map<int, vector<int>> cgroups;
            for (int gi = 0; gi < ng; gi++)
                cgroups[cycle_group.find(gi)].push_back(gi);

            // Within each 2-group: connect tree-groups in a cycle
            for (auto& [cr, gis] : cgroups) {
                if ((int)gis.size() >= 3) {
                    for (int i = 0; i < (int)gis.size(); i++) {
                        int ni = (i + 1) % (int)gis.size();
                        int a = group_list[gis[i]][0];
                        int b = group_list[gis[ni]][0];
                        answer[a][b] = answer[b][a] = 1;
                    }
                } else if ((int)gis.size() == 1) {
                    // Single tree group, no cycle needed
                } else {
                    return 0;
                }
            }

            // Between 2-groups: connect in a meta-cycle for p=3
            vector<int> cgroup_list;
            for (auto& [cr, gis] : cgroups)
                cgroup_list.push_back(cr);
            int ncg = (int)cgroup_list.size();
            if (ncg < 3) return 0;

            for (int i = 0; i < ncg; i++) {
                int ni = (i + 1) % ncg;
                auto& g1 = cgroups[cgroup_list[i]];
                auto& g2 = cgroups[cgroup_list[ni]];
                int a = group_list[g1[0]][0];
                int b = group_list[g2[0]][0];
                answer[a][b] = answer[b][a] = 1;
            }
        } else {
            return 0;
        }
    }

    // Output adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", answer[i][j]);
        printf("\n");
    }
    return 1;
}

int main() {
    int n;
    scanf("%d", &n);
    vector<vector<int>> p(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &p[i][j]);
    int res = construct(p);
    if (!res) printf("Impossible\n");
    return 0;
}
