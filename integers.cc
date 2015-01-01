#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>

using namespace std;

#define FR(i,a,b) for(int i=(a);i<(b);++i)
#define FOR(i,n) FR(i,0,n)

const int SETSZ = 8;
const int MAXSUMCNT = 56;
const int MAXSUBSET = 4;
int nsums[MAXSUBSET];
int sums[MAXSUBSET][MAXSUMCNT];
int soln[SETSZ];

// return true if the sum was already present
bool pushsum(int sz, int tot) {
    // TODO: lookup table would be better
    FOR(j, MAXSUBSET) {
        FOR(i, nsums[j]) {
            if (sums[j][i] == tot) return true;
        }
    }

    sums[sz][nsums[sz]++] = tot;
    return false;
}

int bestk;

set<int> verif;
void doverif(int s) {
    if (verif.count(s) != 0) {
        printf("  VERIFICATION FAILED: sum %d already present\n", s);
    }
    verif.insert(s);
}

void rec(int k, int ub) {
    if (k > bestk) {
        bestk = k;
        printf("       %d: ", k);
        FOR(i, k) {
            printf(" %d", soln[i]);
        }
        printf("\n");

        if (k == 8) {
            printf("SUBSET SUMS:\n");
            
            FOR(i, k) {
                int a = soln[i];
                printf("%2d           = %3d\n", a, a);
                doverif(a);
                FR(j, i+1, k) {
                    int b = soln[j];
                    printf("%2d + %2d      = %3d\n", a, b, a+b);
                    doverif(a+b);
                    FR(l, j+1, k) {
                        int c = soln[l];
                        printf("%2d + %2d + %2d = %3d\n", a, b, c, a+b+c);
                        doverif(a+b+c);
                    }
                }
            }

            exit(0);
        }
    }

    int bot;
    if (k == 0) {
        printf("... %3d\n", ub);
        bot = ub;
    } else {
        // For k=1, the first input is always 0.
        bot = max(soln[0] - soln[k-1], SETSZ - k);
    }

    int oldnsums[MAXSUBSET];
    memcpy(oldnsums, nsums, sizeof(oldnsums));
    for (int cur = bot; cur <= ub; ++cur) {
        for (int oldsz = MAXSUBSET - 2; oldsz >= 0; --oldsz) {
            FOR(i, nsums[oldsz]) {
                if (pushsum(oldsz+1, sums[oldsz][i] + cur)) {
                    goto rejected;
                }
            }
        }

        //printf("=== soln[%d] = %d\n", k, cur);
        soln[k] = cur;
        rec(k+1, cur-1);

        rejected:
        memcpy(nsums, oldnsums, sizeof(nsums));
    }
}

int main() {
    bestk = 0;

    FOR(i,MAXSUBSET) {
        nsums[i] = 0;
        FOR(j,MAXSUMCNT) {
            sums[i][j] = -1;
        }
    }
    pushsum(0, 0);

    for (int ub = 8; ; ++ub) {
        rec(0, ub);
    }

    return 0;
}
