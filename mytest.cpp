//
// Unigine C++ School 2 test task
// by griff
//

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <regex>
#include <unordered_map>

using namespace std;

template <typename T, typename U>
vector<pair<T, U> > vector_from_unordered_map(const unordered_map<T, U> &map) {
    vector<pair<T, U> > out;
    out.reserve(map.size());
    for (auto pair : map) {
        out.push_back(pair);
    }
    return out;
}

template <typename T, typename U>
void print_freqs_vector(const vector<pair<T, U> > &vector, size_t top_n) {
    for (size_t i = 0; i < min(vector.size(), top_n); ++i) {
        auto pair = vector[i];
        cout << pair.second << " " << pair.first << endl;
    }
}

static bool sort_by_freq(const pair<string, size_t> &a, const pair<string, size_t> &b) {
    return ((a.second == b.second) && (a.first < b.first)) || (a.second > b.second);
}

static void usage_error(const char *ex) {
    cerr << "Usage : " << ex << " [-n NNN] <in_file> <out_file>" << endl;
}

int main(int argc, char **argv) try {
    // argument checks
    if (argc != 3 && argc != 5) {
        usage_error(argv[0]);
        return 1;
    }

    size_t top_n = SIZE_MAX;
    size_t url_count = 0;

    int arg_in_file = 1;
    if (string(argv[1]) == "-n") {
        istringstream iss(argv[2]);
        if (!(iss >> top_n)) {
            usage_error(argv[0]); return 1;
        }
        arg_in_file = 3;
    }

    ios_base::sync_with_stdio(false);

    ifstream inf;
    inf.open(argv[arg_in_file]), cin.rdbuf(inf.rdbuf());

    ofstream outf;
    outf.open(argv[arg_in_file + 1]), cout.rdbuf(outf.rdbuf());

    unordered_map<string, size_t> domains;
    unordered_map<string, size_t> paths;

    // iterate over URLs and fill domain and paths maps
    regex url_regex("https?://([a-zA-Z0-9.-]*)(?:/([a-zA-Z0-9.,/+_]*))?");

    for(string s; cin >> s;) {
        sregex_iterator next(s.begin(), s.end(), url_regex);
        sregex_iterator end;
        while (next != end) {
            smatch match = *next;

            string domain = match.str(1);

            string path = "/" + match.str(2);

            domains[domain] += 1;
            paths[path] += 1;

            ++url_count;

            ++next;
        }
    }

    // sort domains & paths
    vector<pair<string, size_t> > domain_freqs = vector_from_unordered_map(domains);
    sort(domain_freqs.begin(), domain_freqs.end(), sort_by_freq);
    vector<pair<string, size_t> > path_freqs = vector_from_unordered_map(paths);
    sort(path_freqs.begin(), path_freqs.end(), sort_by_freq);

    // write to file
    cout << "total urls " << url_count << ", domains " << domains.size() << ", paths " << paths.size() << endl;
    cout << endl;
    cout << "top domains" << endl;
    print_freqs_vector(domain_freqs, top_n);
    cout << endl;

    cout << "top paths" << endl;
    print_freqs_vector(path_freqs, top_n);

    return 0;
} catch (regex_error& e) {
    cerr << e.what();
}

