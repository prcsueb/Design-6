/*
**Time Complexity (TC):**
- `AutocompleteSystem(vector<string>& sentences, vector<int>& times)`: O(N * L log L)
- `input(char c)`: O(L log L)

**Space Complexity (SC):** O(N * L)

Where:
- `N` is the number of sentences.
- `L` is the average length of the sentences.
*/

class AutocompleteSystem {
private:
    struct Compare {
        bool operator()(const pair<int, string> &a, const pair<int, string> &b) const {
            if(a.first == b.first) return a.second < b.second;
            return a.first > b.first;
        }
    };

    struct Node {
        Node* child[27];
        set<pair<int, string>, Compare> sentences;

        Node() {
            for(int i = 0; i < 27; i++) child[i] = NULL;
        }
    };

    const int MAX_MATCH_COUNT = 3;

    string query;
    Node* root, *curRoot;
    unordered_map<string, int> sentenceFreq;

    void insert(string sentence, int freq) {
        Node* curNode = root;

        int oldFreq = sentenceFreq[sentence];
        sentenceFreq[sentence] += freq;
        int newFreq = sentenceFreq[sentence];

        for(char c: sentence) {
            int index = c - 'a';
            if (c == ' ') index = 26;

            if(curNode->child[index] == NULL) {
                curNode->child[index] = new Node();
            }
            curNode = curNode->child[index];

            curNode->sentences.erase({oldFreq, sentence});
            curNode->sentences.insert({newFreq, sentence});

            if(curNode->sentences.size() > 3) curNode->sentences.erase(prev(curNode->sentences.end()));
        }
    }

    vector<string> search(char c) {
        vector<string> matching;

        int index = c - 'a';
        if(c == ' ') index = 26;

        if(curRoot== NULL) return vector<string>();
        curRoot = curRoot->child[index];
        if(curRoot== NULL) return vector<string>();

        int sentencesSoFar = 0;
        for(auto it = curRoot->sentences.begin(); it != curRoot->sentences.end(); it++) {
            matching.push_back(it->second);
            
            sentencesSoFar++;
            if(sentencesSoFar == MAX_MATCH_COUNT) break;
        }

        return matching;
    }

public:
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        root = new Node();
        for(int index = 0; index < times.size(); index++) insert(sentences[index], times[index]);
        query = "";
        curRoot = root;
    }
    
    vector<string> input(char c) {
        if(c == '#') {
            insert(query, 1);
            query = "";
            curRoot = root;
            return vector<string>();
        }

        query += c;
        return search(c);
    }
};