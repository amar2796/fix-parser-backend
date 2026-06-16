#include "crow.h"
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cctype>

// ---------- FIX Tag Dictionary ----------
struct TagInfo {
    std::string name;
    std::unordered_map<std::string, std::string> values;
    std::string why; // plain-English explanation of why this field matters
};

static const std::unordered_map<int, TagInfo>& getDictionary() {
    static const std::unordered_map<int, TagInfo> dict = {
        // ---- Standard Header ----
        {8,   {"BeginString", {}}},
        {9,   {"BodyLength", {}}},
        {35,  {"MsgType", {
            {"0","Heartbeat"},{"1","Test Request"},{"2","Resend Request"},{"3","Reject"},
            {"4","Sequence Reset"},{"5","Logout"},{"6","Indication of Interest"},
            {"7","Advertisement"},{"8","Execution Report"},{"9","Order Cancel Reject"},
            {"A","Logon"},{"B","News"},{"C","Email"},{"D","New Order Single"},
            {"E","New Order List"},{"F","Order Cancel Request"},
            {"G","Order Cancel/Replace Request"},{"H","Order Status Request"},
            {"J","Allocation Instruction"},{"K","List Cancel Request"},
            {"L","List Execute"},{"M","List Status Request"},{"N","List Status"},
            {"P","Allocation Instruction Ack"},{"Q","Don't Know Trade"},
            {"R","Quote Request"},{"S","Quote"},{"T","Settlement Instructions"},
            {"V","Market Data Request"},{"W","Market Data Snapshot/Full Refresh"},
            {"X","Market Data Incremental Refresh"},{"Y","Market Data Request Reject"},
            {"Z","Quote Cancel"},{"a","Quote Status Request"},
            {"b","Mass Quote Acknowledgement"},{"c","Security Definition Request"},
            {"d","Security Definition"},{"e","Security Status Request"},
            {"f","Security Status"},{"g","Trading Session Status Request"},
            {"h","Trading Session Status"},{"i","Mass Quote"},
            {"j","Business Message Reject"},{"k","Bid Request"},
            {"l","Bid Response"},{"m","List Strike Price"},
            {"AE","Trade Capture Report"},{"AF","Order Mass Status Request"},
            {"AG","Quote Request Reject"},{"AJ","Confirmation"},
            {"AK","Settlement Instruction Request"},
        }}},
        {49,  {"SenderCompID", {}}},
        {50,  {"SenderSubID", {}}},
        {56,  {"TargetCompID", {}}},
        {57,  {"TargetSubID", {}}},
        {34,  {"MsgSeqNum", {}}},
        {43,  {"PossDupFlag", {{"Y","Yes (possible duplicate)"},{"N","No"}}}},
        {52,  {"SendingTime", {}}},
        {97,  {"PossResend", {{"Y","Yes"},{"N","No"}}}},
        {115, {"OnBehalfOfCompID", {}}},
        {128, {"DeliverToCompID", {}}},
        {142, {"SenderLocationID", {}}},
        {143, {"TargetLocationID", {}}},
        {369, {"LastMsgSeqNumProcessed", {}}},
        {212, {"XmlDataLen", {}}},
        {213, {"XmlData", {}}},

        // ---- Common Order fields ----
        {1,   {"Account", {}}},
        {6,   {"AvgPx", {}}},
        {11,  {"ClOrdID", {}}},
        {12,  {"Commission", {}}},
        {13,  {"CommType", {{"1","Per unit"},{"2","Percent"},{"3","Absolute"}}}},
        {14,  {"CumQty", {}}},
        {15,  {"Currency", {}}},
        {17,  {"ExecID", {}}},
        {18,  {"ExecInst", {
            {"0","Stay on offerside"},{"1","Not held"},{"2","Work"},
            {"3","Go along"},{"4","Over the day"},{"5","Held"},
            {"6","Participate don't initiate"},{"7","Strict scale"},
            {"G","Insert"},{"I","Try to scale"},{"L","Last peg"}
        }}},
        {19,  {"ExecRefID", {}}},
        {20,  {"ExecTransType", {{"0","New"},{"1","Cancel"},{"2","Correct"},{"3","Status"}}}},
        {21,  {"HandlInst", {
            {"1","Automated execution, no broker intervention"},
            {"2","Automated execution, broker intervention OK"},
            {"3","Manual order"}
        }}},
        {22,  {"IDSource", {
            {"1","CUSIP"},{"2","SEDOL"},{"3","QUIK"},{"4","ISIN"},
            {"5","RIC"},{"6","ISO Currency Code"},{"8","Exchange Symbol"}
        }}},
        {23,  {"IOIID", {}}},
        {25,  {"IOIQltyInd", {{"H","High"},{"L","Low"},{"M","Medium"}}}},
        {27,  {"IOIQty", {}}},
        {28,  {"IOITransType", {{"N","New"},{"C","Cancel"},{"R","Replace"}}}},
        {29,  {"LastCapacity", {{"1","Agent"},{"2","Cross as agent"},{"3","Cross as principal"},{"4","Principal"}}}},
        {30,  {"LastMkt", {}}},
        {31,  {"LastPx", {}}},
        {32,  {"LastShares", {}}},
        {37,  {"OrderID", {}}},
        {38,  {"OrderQty", {}}},
        {39,  {"OrdStatus", {
            {"0","New"},{"1","Partially Filled"},{"2","Filled"},
            {"3","Done for Day"},{"4","Canceled"},{"5","Replaced"},
            {"6","Pending Cancel"},{"7","Stopped"},{"8","Rejected"},
            {"9","Suspended"},{"A","Pending New"},{"B","Calculated"},
            {"C","Expired"},{"D","Accepted for Bidding"},{"E","Pending Replace"}
        }}},
        {40,  {"OrdType", {
            {"1","Market"},{"2","Limit"},{"3","Stop"},{"4","Stop Limit"},
            {"5","Market On Close"},{"6","With Or Without"},{"7","Limit Or Better"},
            {"8","Limit With Or Without"},{"9","On Basis"},{"D","Previously Quoted"},
            {"E","Previously Indicated"},{"P","Pegged"}
        }}},
        {41,  {"OrigClOrdID", {}}},
        {42,  {"OrigTime", {}}},
        {44,  {"Price", {}}},
        {45,  {"RefSeqNum", {}}},
        {48,  {"SecurityID", {}}},
        {54,  {"Side", {
            {"1","Buy"},{"2","Sell"},{"3","Buy minus"},{"4","Sell plus"},
            {"5","Sell short"},{"6","Sell short exempt"},{"7","Undisclosed"},
            {"8","Cross"},{"9","Cross short"}
        }}},
        {55,  {"Symbol", {}}},
        {58,  {"Text", {}}},
        {59,  {"TimeInForce", {
            {"0","Day"},{"1","Good Till Cancel"},{"2","At the Opening"},
            {"3","Immediate or Cancel"},{"4","Fill or Kill"},
            {"5","Good Till Crossing"},{"6","Good Till Date"}
        }}},
        {60,  {"TransactTime", {}}},
        {62,  {"ValidUntilTime", {}}},
        {63,  {"SettlmntTyp", {
            {"0","Regular"},{"1","Cash"},{"2","Next Day"},{"3","T+2"},
            {"4","T+3"},{"5","T+4"},{"6","Future"},{"7","When and if Issued"},
            {"8","Sellers Option"},{"9","T+5"}
        }}},
        {64,  {"FutSettDate", {}}},
        {65,  {"SymbolSfx", {}}},
        {66,  {"ListID", {}}},
        {67,  {"ListSeqNo", {}}},
        {68,  {"TotNoOrders", {}}},
        {69,  {"ListExecInst", {}}},
        {70,  {"AllocID", {}}},
        {71,  {"AllocTransType", {{"0","New"},{"1","Replace"},{"2","Cancel"}}}},
        {72,  {"RefAllocID", {}}},
        {73,  {"NoOrders", {}}},
        {74,  {"AvgPxPrecision", {}}},
        {75,  {"TradeDate", {}}},
        {76,  {"ExecBroker", {}}},
        {77,  {"PositionEffect", {{"O","Open"},{"C","Close"},{"R","Rolled"},{"F","FIFO"}}}},
        {78,  {"NoAllocs", {}}},
        {79,  {"AllocAccount", {}}},
        {80,  {"AllocQty", {}}},
        {81,  {"ProcessCode", {{"0","Regular"},{"1","Soft dollar"},{"2","Step-in"},{"3","Step-out"}}}},
        {82,  {"NoRpts", {}}},
        {83,  {"RptSeq", {}}},
        {84,  {"CxlQty", {}}},
        {85,  {"NoDlvyInst", {}}},
        {87,  {"AllocStatus", {{"0","Accepted"},{"1","Rejected"},{"2","Partial accept"},{"3","Received"}}}},
        {88,  {"AllocRejCode", {}}},
        {89,  {"Signature", {}}},
        {90,  {"SecureDataLen", {}}},
        {91,  {"SecureData", {}}},
        {93,  {"SignatureLength", {}}},
        {94,  {"EmailType", {{"0","New"},{"1","Reply"},{"2","Admin reply"}}}},
        {98,  {"EncryptMethod", {{"0","None / other"},{"1","PKCS"},{"2","DES"},{"3","PKCS/DES"},{"4","PGP/DES"},{"5","PGP/DES-MD5"},{"6","PEM/DES-MD5"}}}},
        {99,  {"StopPx", {}}},
        {100, {"ExDestination", {}}},
        {102, {"CxlRejReason", {{"0","Too late to cancel"},{"1","Unknown order"},{"2","Broker option"},{"3","Order already pending cancel"}}}},
        {103, {"OrdRejReason", {
            {"0","Broker option"},{"1","Unknown symbol"},{"2","Exchange closed"},
            {"3","Order exceeds limit"},{"4","Too late to enter"},{"5","Unknown order"},
            {"6","Duplicate order"},{"8","Stale order"}
        }}},
        {104, {"IOIQualifier", {}}},
        {106, {"Issuer", {}}},
        {107, {"SecurityDesc", {}}},
        {108, {"HeartBtInt", {}}},
        {109, {"ClientID", {}}},
        {110, {"MinQty", {}}},
        {111, {"MaxFloor", {}}},
        {112, {"TestReqID", {}}},
        {113, {"ReportToExch", {{"Y","Yes"},{"N","No"}}}},
        {114, {"LocateReqd", {{"Y","Yes"},{"N","No"}}}},
        {116, {"OnBehalfOfSubID", {}}},
        {117, {"QuoteID", {}}},
        {118, {"NetMoney", {}}},
        {119, {"SettlCurrAmt", {}}},
        {120, {"SettlCurrency", {}}},
        {121, {"ForexReq", {{"Y","Yes"},{"N","No"}}}},
        {122, {"OrigSendingTime", {}}},
        {123, {"GapFillFlag", {{"Y","Yes"},{"N","No"}}}},
        {124, {"NoExecs", {}}},
        {126, {"ExpireTime", {}}},
        {127, {"DKReason", {
            {"A","Unknown symbol"},{"B","Wrong side"},{"C","Quantity exceeds order"},
            {"D","No matching order"},{"E","Price exceeds limit"},{"Z","Other"}
        }}},
        {129, {"DeliverToSubID", {}}},
        {130, {"IOINaturalFlag", {{"Y","Yes"},{"N","No"}}}},
        {131, {"QuoteReqID", {}}},
        {132, {"BidPx", {}}},
        {133, {"OfferPx", {}}},
        {134, {"BidSize", {}}},
        {135, {"OfferSize", {}}},
        {136, {"NoMiscFees", {}}},
        {137, {"MiscFeeAmt", {}}},
        {138, {"MiscFeeCurr", {}}},
        {139, {"MiscFeeType", {{"1","Regulatory"},{"2","Tax"},{"3","Local commission"},{"4","Exchange fees"}}}},
        {140, {"PrevClosePx", {}}},
        {141, {"ResetSeqNumFlag", {{"Y","Yes"},{"N","No"}}}},
        {144, {"NoRelatedSym", {}}},
        {146, {"NoRelatedSym", {}}},
        {150, {"ExecType", {
            {"0","New"},{"1","Partial Fill"},{"2","Fill"},{"3","Done for Day"},
            {"4","Canceled"},{"5","Replaced"},{"6","Pending Cancel"},
            {"7","Stopped"},{"8","Rejected"},{"9","Suspended"},
            {"A","Pending New"},{"B","Calculated"},{"C","Expired"},
            {"D","Restated"},{"E","Pending Replace"},{"F","Trade"},
            {"G","Trade Correct"},{"H","Trade Cancel"},{"I","Order Status"}
        }}},
        {151, {"LeavesQty", {}}},
        {152, {"CashOrderQty", {}}},
        {161, {"AllocText", {}}},
        {167, {"SecurityType", {
            {"CS","Common Stock"},{"PS","Preferred Stock"},{"FUT","Future"},
            {"OPT","Option"},{"FOR","Foreign Exchange"},{"CORP","Corporate Bond"},
            {"CB","Convertible Bond"},{"MLEG","Multileg"},{"BOND","Bond"}
        }}},
        {172, {"SettlInstMode", {}}},
        {198, {"SecondaryOrderID", {}}},
        {200, {"MaturityMonthYear", {}}},
        {201, {"PutOrCall", {{"0","Put"},{"1","Call"}}}},
        {202, {"StrikePrice", {}}},
        {204, {"CustomerOrFirm", {{"0","Customer"},{"1","Firm"}}}},
        {206, {"OptAttribute", {}}},
        {207, {"SecurityExchange", {}}},
        {223, {"CouponRate", {}}},
        {231, {"ContractMultiplier", {}}},
        {262, {"MDReqID", {}}},
        {263, {"SubscriptionRequestType", {{"0","Snapshot"},{"1","Snapshot+Updates"},{"2","Unsubscribe"}}}},
        {264, {"MarketDepth", {}}},
        {265, {"MDUpdateType", {{"0","Full Refresh"},{"1","Incremental Refresh"}}}},
        {266, {"AggregatedBook", {{"Y","Yes"},{"N","No"}}}},
        {267, {"NoMDEntryTypes", {}}},
        {268, {"NoMDEntries", {}}},
        {269, {"MDEntryType", {
            {"0","Bid"},{"1","Offer"},{"2","Trade"},{"3","Index Value"},
            {"4","Opening Price"},{"5","Closing Price"},{"6","Settlement Price"},
            {"7","Trading Session High Price"},{"8","Trading Session Low Price"},
            {"9","Trading Session VWAP Price"}
        }}},
        {270, {"MDEntryPx", {}}},
        {271, {"MDEntrySize", {}}},
        {272, {"MDEntryDate", {}}},
        {273, {"MDEntryTime", {}}},
        {274, {"TickDirection", {{"0","Plus tick"},{"1","Zero-plus tick"},{"2","Minus tick"},{"3","Zero-minus tick"}}}},
        {275, {"MDMkt", {}}},
        {276, {"QuoteCondition", {}}},
        {277, {"TradeCondition", {}}},
        {278, {"MDEntryID", {}}},
        {279, {"MDUpdateAction", {{"0","New"},{"1","Change"},{"2","Delete"}}}},
        {280, {"MDEntryRefID", {}}},
        {281, {"MDReqRejReason", {
            {"0","Unknown symbol"},{"1","Duplicate MDReqID"},{"2","Insufficient bandwidth"},
            {"3","Insufficient permissions"},{"4","Unsupported SubscriptionRequestType"}
        }}},
        {282, {"MDEntryOriginator", {}}},
        {283, {"LocationID", {}}},
        {284, {"DeskID", {}}},
        {286, {"OpenCloseSettlFlag", {{"0","Daily open"},{"1","Session open"},{"2","Delivery settlement"}}}},
        {288, {"ActiveFlag", {}}},
        {289, {"IndicativeAuctionFlag", {}}},
        {290, {"TradSesReqID", {}}},
        {291, {"TradSesMethod", {{"1","Electronic"},{"2","Open outcry"},{"3","Two party"}}}},
        {292, {"TradSesMode", {{"1","Testing"},{"2","Simulated"},{"3","Production"}}}},
        {294, {"SubscriptionRequestType", {}}},
        {297, {"TradSesStatus", {{"1","Halted"},{"2","Open"},{"3","Closed"},{"4","Pre-Open"},{"5","Pre-Close"}}}},
        {299, {"QuoteEntryID", {}}},
        {300, {"QuoteRejectReason", {}}},
        {301, {"QuoteResponseLevel", {}}},
        {302, {"QuoteSetID", {}}},
        {303, {"QuoteRequestType", {{"1","Manual"},{"2","Automatic"}}}},
        {304, {"TotNoQuoteEntries", {}}},
        {305, {"UnderlyingIDSource", {}}},
        {307, {"UnderlyingSymbol", {}}},
        {336, {"TradingSessionID", {}}},
        {363, {"AllocLinkID", {}}},
        {372, {"RefMsgType", {}}},
        {373, {"SessionRejectReason", {
            {"0","Invalid tag number"},{"1","Required tag missing"},
            {"2","Tag not defined for this message type"},{"3","Undefined tag"},
            {"4","Tag specified without a value"},{"5","Value is incorrect"},
            {"6","Incorrect data format for value"},{"7","Decryption problem"},
            {"8","Signature problem"},{"9","CompID problem"},
            {"10","SendingTime accuracy problem"},{"11","Invalid MsgType"}
        }}},
        {380, {"BusinessRejectReason", {
            {"0","Other"},{"1","Unknown ID"},{"2","Unknown Security"},
            {"3","Unsupported Message Type"},{"4","Application not available"},
            {"5","Conditionally Required Field Missing"}
        }}},
        {382, {"NoContraBrokers", {}}},
        {423, {"PriceType", {{"1","Percentage"},{"2","Per unit"},{"3","Fixed amount"},{"6","Yield"}}}},
        {432, {"ExpireDate", {}}},
        {434, {"CxlRejResponseTo", {{"1","Order Cancel Request"},{"2","Order Cancel/Replace Request"}}}},
        {442, {"MultiLegReportingType", {{"1","Single Security"},{"2","Individual leg"},{"3","Multileg security"}}}},
        {447, {"PartyIDSource", {}}},
        {448, {"PartyID", {}}},
        {452, {"PartyRole", {
            {"1","Executing Firm"},{"2","Broker of Credit"},{"3","Client ID"},
            {"4","Clearing Firm"},{"5","Investor ID"},{"11","Order Origination Firm"}
        }}},
        {453, {"NoPartyIDs", {}}},
        {454, {"NoSecurityAltID", {}}},
        {460, {"Product", {}}},
        {461, {"CFICode", {}}},
        {462, {"UnderlyingProduct", {}}},
        {487, {"TradeReportTransType", {{"0","New"},{"1","Cancel"},{"2","Replace"}}}},
        {527, {"SecondaryExecID", {}}},
        {528, {"OrderCapacity", {{"A","Agency"},{"G","Proprietary"},{"I","Individual"},{"P","Principal"},{"R","Riskless Principal"},{"W","Agent for Other Member"}}}},
        {529, {"OrderRestrictions", {}}},
        {552, {"NoSides", {}}},
        {555, {"NoLegs", {}}},
        {556, {"LegCurrency", {}}},
        {581, {"AccountType", {
            {"1","Carried customer side"},{"2","House trader"},{"3","Floor trader"},
            {"4","Carried customer side, cross margin"},{"5","Customer cross margin account"}
        }}},
        {584, {"MassStatusReqID", {}}},
        {585, {"MassStatusReqType", {{"1","Status for orders for a security"},{"7","Status for all orders"}}}},
        {608, {"ClearingFeeIndicator", {}}},
        {624, {"LegSide", {{"1","Buy"},{"2","Sell"}}}},
        {636, {"WorkingIndicator", {{"Y","Yes"},{"N","No"}}}},
        {663, {"MidPx", {}}},
        {711, {"NoUnderlyings", {}}},
        {797, {"CopyMsgIndicator", {{"Y","Yes"},{"N","No"}}}},
        {893, {"LastFragment", {{"Y","Yes"},{"N","No"}}}},
        {925, {"DisregardFlag", {}}},

        // ---- Trailer ----
        {10,  {"CheckSum", {}}},
    };
    return dict;
}

// Plain-English "why this field matters" explanations for the most common/important tags.
// Used by the frontend walkthrough mode to teach, not just display.
static const std::unordered_map<int, std::string>& getWhyExplanations() {
    static const std::unordered_map<int, std::string> why = {
        {8,   "Identifies which version of the FIX protocol this message follows. Both sides must agree on this or the message can't be interpreted correctly."},
        {9,   "Tells the receiver exactly how many bytes to expect in the message body. If this doesn't match, the message is considered corrupt."},
        {35,  "The single most important field in any FIX message — it tells the receiving system what KIND of message this is (an order, a fill, a cancel, etc.), which determines how every other field should be interpreted."},
        {49,  "Identifies who is sending this message. Trading systems use this to route messages and apply the correct permissions/rules for that sender."},
        {56,  "Identifies who this message is being sent TO. Combined with SenderCompID, this forms the unique 'conversation' between two trading parties."},
        {34,  "A sequence number that increases with every message. If a number is skipped, the receiver knows a message was lost and can request it again."},
        {52,  "The exact timestamp the message was sent. Used to detect stale messages and to reconstruct the true order of events during disputes."},
        {11,  "The unique ID YOU assign to your order. This is how you (the client) refer back to this specific order in every future message about it — cancels, fills, replaces all reference this ID."},
        {55,  "The ticker/instrument being traded. Get this wrong and you could buy or sell the completely wrong asset."},
        {54,  "Tells the counterparty whether you're buying or selling. Get this backwards and the trade direction is reversed — a costly mistake."},
        {38,  "How many shares/contracts/units you want to trade. This directly determines the size of your financial exposure."},
        {40,  "Tells the exchange HOW to execute your order — at any price (Market), only at your specified price or better (Limit), or only after a trigger price is hit (Stop)."},
        {44,  "The price you're willing to trade at. Only meaningful for Limit and Stop-Limit orders — Market orders execute at whatever price is available."},
        {59,  "Tells the exchange how long your order should remain active — for the rest of the day, until cancelled, or it must fill immediately or be killed."},
        {60,  "The exact time the trading decision was made (not when the message was sent). Regulators and exchanges use this for audit trails and timestamp accuracy checks."},
        {37,  "The unique ID the EXCHANGE (not you) assigns to your order once it's accepted. You'll see this ID in all future updates about this order from the exchange's side."},
        {39,  "Tells you the current lifecycle state of your order — is it brand new, partially filled, completely filled, or canceled? This is how you track an order's progress."},
        {150, "Tells you specifically WHAT just happened to cause this Execution Report — was it a new acknowledgment, a partial fill, a full fill, or a cancellation?"},
        {17,  "A unique ID for this specific execution event (like a single fill). Useful for reconciling trades against your own records."},
        {14,  "The total quantity that has been filled so far across all fills for this order. Helps you track how much of your order remains unfilled."},
        {151, "How much of your order quantity is still unfilled and working in the market right now."},
        {6,   "The average price across all fills for this order so far — useful when an order fills in multiple pieces at different prices."},
        {41,  "When canceling or replacing an order, this points back to the ClOrdID of the order you're trying to modify. Without this, the exchange wouldn't know which order you mean."},
        {1,   "Identifies which trading account this order belongs to — important for firms managing multiple client accounts through the same connection."},
        {21,  "Tells the broker how much they're allowed to intervene with your order — fully automated, automated with human oversight, or fully manual."},
        {98,  "Specifies whether and how this connection's messages are encrypted. Most modern connections use 'None' since encryption is handled at the network layer instead."},
        {108, "How many seconds of silence are allowed before each side must send a Heartbeat to prove the connection is still alive."},
        {112, "A reference ID used when one side asks the other 'are you still there?' — the response must echo this same ID back."},
        {45,  "Points to the sequence number (tag 34) of the message that caused this Reject — tells you exactly which message had the problem."},
        {372, "Identifies which MESSAGE TYPE caused a Reject or Business Reject — helps pinpoint what kind of message the receiver couldn't process."},
        {373, "Gives the specific technical REASON a message was rejected at the session level — e.g. a required tag was missing, or a value was invalid."},
        {380, "Gives the specific business-level reason a message was rejected — different from a session reject, this means the message was understood but couldn't be processed for a business reason."},
        {262, "A unique ID for this specific market data request — the exchange includes this same ID in its responses so you know which request they're answering."},
        {268, "Tells the receiver how many individual market data entries (price levels, trades, etc.) follow in this message. This is what makes 'repeating groups' work."},
        {269, "Tells you what KIND of market data this entry represents — a bid price, an offer price, a trade, etc."},
        {270, "The actual price for this market data entry — a bid price, offer price, or trade price depending on the entry type."},
        {271, "The size/quantity available at this price level (for quotes) or the size that traded (for trade entries)."},
        {453, "Tells the receiver how many party identification blocks (broker, client, firm, etc.) follow in this message."},
        {448, "Identifies a specific party (a person, firm, or system) involved in this transaction — combined with PartyRole to clarify exactly what role they played."},
        {452, "Clarifies WHAT ROLE the associated PartyID plays — are they the executing firm, the client, the clearing firm? Critical for regulatory reporting."},
        {102, "Explains exactly why your cancel request was rejected — e.g. it was too late to cancel, or the exchange doesn't recognize that order."},
        {103, "Explains exactly why your new order was rejected outright — e.g. unknown symbol, market closed, or order size exceeds allowed limits."},
        {58,  "Free-text field used for human-readable explanations — often holds the specific reason behind a rejection or status change when the structured fields aren't detailed enough."},
        {15,  "Specifies the currency the price and amounts in this message are denominated in — critical for any cross-currency or international trading."},
        {528, "Tells the exchange in what capacity you're trading — as an agent for someone else, or as a principal trading your own firm's capital. This affects margin and regulatory treatment."},
        {10,  "A mathematical checksum of the entire message. If even one character gets corrupted in transit, this number won't match anymore — it's the message's built-in tamper/corruption detector."},
    };
    return why;
}


// Tags that belong to the FIX Header
static bool isHeaderTag(int tag) {
    static const std::unordered_set<int> headerTags = {
        8, 9, 35, 49, 50, 56, 57, 34, 43, 52, 97, 115, 128, 142, 143, 369, 212, 213
    };
    return headerTags.count(tag) > 0;
}

static bool isTrailerTag(int tag) {
    return tag == 10 || tag == 89 || tag == 93;
}

// Repeating group counter tags -> the tag that starts each repeated entry
// (count tag -> first tag of each group instance). Used to detect/group repeats.
static const std::unordered_map<int, int>& getGroupStartTags() {
    static const std::unordered_map<int, int> groups = {
        {73, 11},    // NoOrders -> ClOrdID (simplified)
        {78, 79},    // NoAllocs -> AllocAccount
        {136, 137},  // NoMiscFees -> MiscFeeAmt
        {268, 269},  // NoMDEntries -> MDEntryType
        {267, 269},  // NoMDEntryTypes -> MDEntryType
        {453, 448},  // NoPartyIDs -> PartyID
        {555, 624},  // NoLegs -> LegSide
        {552, 54},   // NoSides -> Side
        {382, 448},  // NoContraBrokers -> PartyID (approx)
    };
    return groups;
}

// Required fields per MsgType (simplified but covering common business messages)
static const std::unordered_map<std::string, std::vector<int>>& getRequiredFieldsByMsgType() {
    static const std::unordered_map<std::string, std::vector<int>> req = {
        {"D",  {11, 55, 54, 38, 40, 60}},                  // New Order Single
        {"8",  {37, 11, 17, 150, 39, 55, 54, 14, 6}},       // Execution Report
        {"F",  {41, 11, 55, 54, 60}},                       // Order Cancel Request
        {"G",  {41, 11, 55, 54, 60}},                       // Order Cancel/Replace Request
        {"9",  {11, 41, 39, 102}},                          // Order Cancel Reject
        {"A",  {98, 108}},                                  // Logon
        {"5",  {}},                                         // Logout
        {"0",  {}},                                         // Heartbeat
        {"1",  {112}},                                      // Test Request
        {"3",  {45, 372, 373}},                             // Reject
        {"j",  {372, 380}},                                 // Business Message Reject
        {"V",  {262, 263, 264, 265}},                        // Market Data Request
        {"W",  {262, 268}},                                 // Market Data Snapshot
        {"R",  {131}},                                      // Quote Request
    };
    return req;
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
        const auto& groupStarts = getGroupStartTags();

        crow::json::wvalue result;
        result["status"] = "ok";

        std::vector<crow::json::wvalue> headerArr;
        std::vector<crow::json::wvalue> bodyArr;
        std::vector<crow::json::wvalue> trailerArr;
        std::vector<crow::json::wvalue> sequence; // every field in original order, annotated

        std::string msgType = "";
        std::unordered_set<int> groupStartTagSet;
        for (auto& kv : groupStarts) groupStartTagSet.insert(kv.second);

        int currentGroupIndex = -1;
        int currentGroupCountTag = -1;

        for (size_t idx = 0; idx < tokens.size(); ++idx) {
            const auto& tok = tokens[idx];
            crow::json::wvalue entry;
            entry["tag"] = tok.tag;

            std::string rawVal(tok.value);
            entry["raw"] = rawVal;

            auto it = dict.find(tok.tag);
            std::string fieldName;
            if (it != dict.end()) {
                fieldName = it->second.name;
                entry["name"] = fieldName;
                auto vIt = it->second.values.find(rawVal);
                if (vIt != it->second.values.end()) {
                    entry["meaning"] = vIt->second;
                } else {
                    entry["meaning"] = rawVal;
                }
            } else {
                fieldName = "Unknown(" + std::to_string(tok.tag) + ")";
                entry["name"] = fieldName;
                entry["meaning"] = rawVal;
            }

            // Plain-English "why this matters" explanation, for teaching mode
            const auto& whyMap = getWhyExplanations();
            auto whyIt = whyMap.find(tok.tag);
            if (whyIt != whyMap.end()) {
                entry["why"] = whyIt->second;
            } else {
                entry["why"] = "Part of this message's data — not one of the most commonly explained fields, but still required by the FIX dictionary for this tag.";
            }


            // Detect if this tag is a repeating-group counter
            bool isGroupCounter = groupStarts.count(tok.tag) > 0;
            entry["isGroupCounter"] = isGroupCounter;

            // Detect if this tag starts a new repeating group instance
            bool isGroupStart = groupStartTagSet.count(tok.tag) > 0;
            entry["isGroupStart"] = isGroupStart;
            if (isGroupStart) {
                currentGroupIndex++;
            }
            // Only tag group membership while we're inside body/group fields (not header/trailer)
            bool withinGroupableSection = !isHeaderTag(tok.tag) && !isTrailerTag(tok.tag);
            entry["groupIndex"] = (withinGroupableSection && currentGroupIndex >= 0) ? currentGroupIndex : -1;

            if (tok.tag == 35) {
                msgType = rawVal;
            }

            // step index for "loop by loop" frontend walkthrough
            entry["stepIndex"] = static_cast<int>(idx);

            crow::json::wvalue entryCopy1 = entry; // for sequence array
            crow::json::wvalue entryCopy2 = entry; // for section array (header/body/trailer)

            if (isHeaderTag(tok.tag)) {
                headerArr.push_back(std::move(entryCopy2));
            } else if (isTrailerTag(tok.tag)) {
                trailerArr.push_back(std::move(entryCopy2));
            } else {
                bodyArr.push_back(std::move(entryCopy2));
            }

            sequence.push_back(std::move(entryCopy1));
        }

        // ---------- Validation ----------
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

        // Required fields per message type
        const auto& reqMap = getRequiredFieldsByMsgType();
        auto reqIt = reqMap.find(msgType);
        if (reqIt != reqMap.end()) {
            for (int reqTag : reqIt->second) {
                bool found = false;
                for (const auto& tok : tokens) {
                    if (tok.tag == reqTag) { found = true; break; }
                }
                if (!found) {
                    std::string name = dict.count(reqTag) ? dict.at(reqTag).name : std::to_string(reqTag);
                    std::string msgTypeName = dict.count(35) && dict.at(35).values.count(msgType)
                        ? dict.at(35).values.at(msgType) : msgType;
                    errors.push_back(crow::json::wvalue(
                        "Missing required field for " + msgTypeName + ": " + name + " (tag " + std::to_string(reqTag) + ")"));
                    isValid = false;
                }
            }
        }

        // ---------- Assemble Response ----------
        result["delimiterDetected"] = std::string(1, delim == '\x01' ? '^' : delim);
        result["isValid"] = isValid;
        result["validationErrors"] = std::move(errors);
        result["msgType"] = msgType;
        if (dict.count(35) && dict.at(35).values.count(msgType)) {
            result["msgTypeName"] = dict.at(35).values.at(msgType);
        } else {
            result["msgTypeName"] = msgType.empty() ? "Unknown" : ("Unknown MsgType: " + msgType);
        }

        crow::json::wvalue components;
        components["header"] = std::move(headerArr);
        components["body"] = std::move(bodyArr);
        components["trailer"] = std::move(trailerArr);
        result["components"] = std::move(components);

        result["sequence"] = std::move(sequence);
        result["totalFields"] = static_cast<int>(tokens.size());

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
        return "FIX Parser Backend v2 is running";
    });

    app.port(18080).multithreaded().run();
}
