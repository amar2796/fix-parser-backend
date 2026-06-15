#include "crow.h"
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <cctype>

// ---------- FIX Tag Dictionary ----------
struct TagInfo {
    std::string name;
    std::unordered_map<std::string, std::string> values;
};

static const std::unordered_map<int, TagInfo>& getDictionary() {
    static const std::unordered_map<int, TagInfo> dict = {
        {8,  {"BeginString", {}}},
        {9,  {"BodyLength", {}}},
        {35, {"MsgType", {
            {"D", "New Order Single"},
            {"8", "Execution Report"},
            {"0", "Heartbeat"},
            {"1", "Test Request"},
            {"2", "Resend Request"},
            {"3", "Reject"},
            {"4", "Sequence Reset"},
            {"5", "Logout"},
            {"A", "Logon"},
            {"F", "Order Cancel Request"},
            {"G", "Order Cancel/Replace Request"},
            {"9", "Order Cancel Reject"}
        }}},
        {49, {"SenderCompID", {}}},
        {56, {"TargetCompID", {}}},
        {34, {"MsgSeqNum", {}}},
        {52, {"SendingTime", {}}},
        {11, {"ClOrdID", {}}},
        {55, {"Symbol", {}}},
        {54, {"Side", {
            {"1", "Buy"},
            {"2", "Sell"},
            {"3", "Buy minus"},
            {"4", "Sell plus"},
            {"5", "Sell short"},
            {"6", "Sell short exempt"}
        }}},
        {38, {"OrderQty", {}}},
        {40, {"OrdType", {
            {"1", "Market"},
            {"2", "Limit"},
            {"3", "Stop"},
            {"4", "Stop Limit"},
            {"5", "Market On Close"},
            {"P", "Pegged"}
        }}},
        {44, {"Price", {}}},
        {59, {"TimeInForce", {
            {"0", "Day"},
            {"1", "Good Till Cancel"},
            {"2", "At the Opening"},
            {"3", "Immediate or Cancel"},
            {"4", "Fill or Kill"},
            {"6", "Good Till Date"}
        }}},
        {1,  {"Account", {}}},
        {21, {"HandlInst", {
            {"1", "Automated execution, no broker intervention"},
            {"2", "Automated execution, broker intervention OK"},
            {"3", "Manual order"}
        }}},
        {37, {"OrderID", {}}},
        {39, {"OrdStatus", {
            {"0", "New"},
            {"1", "Partially Filled"},
            {"2", "Filled"},
            {"4", "Canceled"},
            {"8", "Rejected"},
            {"A", "Pending New"},
            {"E", "Pending Replace"}
        }}},
        {150, {"ExecType", {
            {"0", "New"},
            {"1", "Partial Fill"},
            {"2", "Fill"},
            {"4", "Canceled"},
            {"8", "Rejected"}
        }}},
        {151, {"LeavesQty", {}}},
        {14,  {"CumQty", {}}},
        {6,   {"AvgPx", {}}},
        {10, {"CheckSum", {}}}
    };
    return dict;
}

static bool isHeaderTag(int tag) {
    switch (tag) {
        case 8: case 9: case 35: case 49: case 56: case 34: case 52:
        case 50: case 57: case 115: case 128: case 142:
            return true;
        default:
            return false;
    }
}

static bool isTrailerTag(int tag) {
    return tag == 10 || tag == 89 || tag == 93;
}

struct FixToken {
    int tag;
    std::string_view value;
};

static char detectDelimiter(const std::string& msg) {
    if (msg.find('\x01') != std::string::npos) return '\x01';
    if (msg.find('|') != std::string::npos) return '|';
    if (msg.find('^') != std::string::npos) return '^';
    if (msg.find(';') != std::string::npos) return ';';
    return '|';
}

static std::vector<FixToken> tokenize(const std::string& msg, char delim) {
    std::vector<FixToken> tokens;
    size_t pos = 0;
    size_t len = msg.size();

    while (pos < len) {
        size_t eqPos = msg.find('=', pos);
        if (eqPos == std::string::npos) break;

        size_t delimPos = msg.find(delim, eqPos + 1);
        if (delimPos == std::string::npos) delimPos = len;

        std::string tagStr = msg.substr(pos, eqPos - pos);
        int tag = 0;
        try {
            tag = std::stoi(tagStr);
        } catch (...) {
            pos = delimPos + 1;
            continue;
        }

        std::string_view value(msg.data() + eqPos + 1, delimPos - (eqPos + 1));
        tokens.push_back({tag, value});

        pos = delimPos + 1;
    }

    return tokens;
}

static int calculateChecksum(const std::string& msg, char delim) {
    size_t checksumTagPos = std::string::npos;

    for (size_t i = 0; i + 3 <= msg.size(); ++i) {
        if (msg[i] == '1' && msg[i+1] == '0' && msg[i+2] == '=') {
            if (i == 0 || msg[i-1] == delim) {
                checksumTagPos = i;
            }
        }
    }

    size_t sumUpTo = (checksumTagPos != std::string::npos) ? checksumTagPos : msg.size();

    unsigned int sum = 0;
    for (size_t i = 0; i < sumUpTo; ++i) {
        sum += static_cast<unsigned char>(msg[i]);
    }
    return static_cast<int>(sum % 256);
}

static std::string padChecksum(int val) {
    std::string s = std::to_string(val);
    while (s.size() < 3) s = "0" + s;
    return s;
}

static int calculateBodyLength(const std::string& msg, char delim) {
    size_t tag9Pos = std::string::npos;
    for (size_t i = 0; i + 2 <= msg.size(); ++i) {
        if (msg[i] == '9' && msg[i+1] == '=') {
            if (i == 0 || msg[i-1] == delim) {
                tag9Pos = i;
                break;
            }
        }
    }
    if (tag9Pos == std::string::npos) return -1;

    size_t valStart = tag9Pos + 2;
    size_t valEnd = msg.find(delim, valStart);
    if (valEnd == std::string::npos) return -1;

    size_t bodyStart = valEnd + 1;

    size_t tag10Pos = std::string::npos;
    for (size_t i = 0; i + 3 <= msg.size(); ++i) {
        if (msg[i] == '1' && msg[i+1] == '0' && msg[i+2] == '=') {
            if (i == 0 || msg[i-1] == delim) {
                tag10Pos = i;
            }
        }
    }

    size_t bodyEnd = (tag10Pos != std::string::npos) ? tag10Pos : msg.size();
    if (bodyEnd < bodyStart) return -1;

    return static_cast<int>(bodyEnd - bodyStart);
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/parse").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        crow::response response;
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Access-Control-Allow-Headers", "Content-Type");
        response.add_header("Content-Type", "application/json");

        if (body.empty()) {
            crow::json::wvalue err;
            err["status"] = "error";
            err["message"] = "Empty input";
            response.body = err.dump();
            response.code = 400;
            return response;
        }

        char delim = detectDelimiter(body);
        std::vector<FixToken> tokens = tokenize(body, delim);

        const auto& dict = getDictionary();

        crow::json::wvalue result;
        result["status"] = "ok";

        std::vector<crow::json::wvalue> headerArr;
        std::vector<crow::json::wvalue> bodyArr;
        std::vector<crow::json::wvalue> trailerArr;

        std::string msgType = "";

        for (const auto& tok : tokens) {
            crow::json::wvalue entry;
            entry["tag"] = tok.tag;

            std::string rawVal(tok.value);
            entry["raw"] = rawVal;

            auto it = dict.find(tok.tag);
            if (it != dict.end()) {
                entry["name"] = it->second.name;
                auto vIt = it->second.values.find(rawVal);
                if (vIt != it->second.values.end()) {
                    entry["meaning"] = vIt->second;
                } else {
                    entry["meaning"] = rawVal;
                }
            } else {
                entry["name"] = ("Unknown(" + std::to_string(tok.tag) + ")");
                entry["meaning"] = rawVal;
            }

            if (tok.tag == 35) {
                msgType = rawVal;
            }

            if (isHeaderTag(tok.tag)) {
                headerArr.push_back(std::move(entry));
            } else if (isTrailerTag(tok.tag)) {
                trailerArr.push_back(std::move(entry));
            } else {
                bodyArr.push_back(std::move(entry));
            }
        }

        std::vector<crow::json::wvalue> errors;
        bool isValid = true;

        int calcChecksum = calculateChecksum(body, delim);
        std::string calcChecksumStr = padChecksum(calcChecksum);

        std::string actualChecksum = "";
        for (const auto& tok : tokens) {
            if (tok.tag == 10) {
                actualChecksum = std::string(tok.value);
                break;
            }
        }

        if (actualChecksum.empty()) {
            errors.push_back(crow::json::wvalue("Missing CheckSum (tag 10)"));
            isValid = false;
        } else if (actualChecksum != calcChecksumStr) {
            errors.push_back(crow::json::wvalue(
                "CheckSum mismatch: expected " + calcChecksumStr + " but got " + actualChecksum));
            isValid = false;
        }

        int calcBodyLen = calculateBodyLength(body, delim);
        std::string actualBodyLen = "";
        for (const auto& tok : tokens) {
            if (tok.tag == 9) {
                actualBodyLen = std::string(tok.value);
                break;
            }
        }

        if (actualBodyLen.empty()) {
            errors.push_back(crow::json::wvalue("Missing BodyLength (tag 9)"));
            isValid = false;
        } else {
            try {
                int actualLenInt = std::stoi(actualBodyLen);
                if (calcBodyLen != actualLenInt) {
                    errors.push_back(crow::json::wvalue(
                        "BodyLength mismatch: expected " + std::to_string(calcBodyLen) +
                        " but got " + std::to_string(actualLenInt)));
                    isValid = false;
                }
            } catch (...) {
                errors.push_back(crow::json::wvalue("Invalid BodyLength value"));
                isValid = false;
            }
        }

        if (msgType == "D") {
            std::vector<int> required = {11, 55, 54, 38, 40};
            for (int reqTag : required) {
                bool found = false;
                for (const auto& tok : tokens) {
                    if (tok.tag == reqTag) { found = true; break; }
                }
                if (!found) {
                    const auto& d = getDictionary();
                    std::string name = d.count(reqTag) ? d.at(reqTag).name : std::to_string(reqTag);
                    errors.push_back(crow::json::wvalue(
                        "Missing required field for NewOrderSingle: " + name + " (tag " + std::to_string(reqTag) + ")"));
                    isValid = false;
                }
            }
        }

        result["delimiterDetected"] = std::string(1, delim == '\x01' ? '^' : delim);
        result["isValid"] = isValid;
        result["validationErrors"] = std::move(errors);

        crow::json::wvalue components;
        components["header"] = std::move(headerArr);
        components["body"] = std::move(bodyArr);
        components["trailer"] = std::move(trailerArr);
        result["components"] = std::move(components);

        crow::json::wvalue checksumInfo;
        checksumInfo["calculated"] = calcChecksumStr;
        checksumInfo["actual"] = actualChecksum;
        result["checksum"] = std::move(checksumInfo);

        crow::json::wvalue bodyLenInfo;
        bodyLenInfo["calculated"] = calcBodyLen;
        bodyLenInfo["actual"] = actualBodyLen;
        result["bodyLength"] = std::move(bodyLenInfo);

        response.body = result.dump();
        response.code = 200;
        return response;
    });

    CROW_ROUTE(app, "/api/parse").methods("OPTIONS"_method)
    ([](const crow::request&) {
        crow::response response(204);
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        response.add_header("Access-Control-Allow-Headers", "Content-Type");
        return response;
    });

    CROW_ROUTE(app, "/")([]() {
        return "FIX Parser Backend is running";
    });

    app.port(18080).multithreaded().run();
}
