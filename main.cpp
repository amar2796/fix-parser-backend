// 
#include "crow.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/api/parse").methods("POST"_method)
    ([](const crow::request& req) {
        std::string body = req.body;

        crow::json::wvalue res;
        res["status"] = "ok";
        res["receivedLength"] = body.size();
        res["echo"] = body;

        crow::response response(res);
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Access-Control-Allow-Headers", "Content-Type");
        return response;
    });

    // Handle CORS preflight
    CROW_ROUTE(app, "/api/parse").methods("OPTIONS"_method)
    ([](const crow::request&) {
        crow::response response(204);
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        response.add_header("Access-Control-Allow-Headers", "Content-Type");
        return response;
    });

    app.port(18080).multithreaded().run();
}