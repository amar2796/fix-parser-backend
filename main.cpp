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

        // ---- Additional commonly used tags (settlement, parties, allocations, quoting, execution detail) ----
        {16,  {"EndSeqNo", {}}},
        {24,  {"IOIOthSvc", {}}},
        {26,  {"IOINaturalFlag", {{"Y","Yes"},{"N","No"}}}},
        {33,  {"NoLinesOfText", {}}},
        {36,  {"NewSeqNo", {}}},
        {46,  {"RelatdSym", {}}},
        {47,  {"Rule80A", {
            {"A","Agency"},{"P","Principal"},{"I","Individual"},{"W","All other orders"},
            {"J","Specialist trades"},{"K","Program trades"},{"R","Riskless principal"}
        }}},
        {53,  {"Quantity", {}}},
        {61,  {"Urgency", {{"0","Normal"},{"1","Flash"},{"2","Background"}}}},
        {86,  {"DKReason", {}}},
        {92,  {"BrokerOfCredit", {}}},
        {95,  {"RawDataLength", {}}},
        {96,  {"RawData", {}}},
        {101, {"BidType", {{"1","Non-disclosed style"},{"2","Disclosed style"},{"3","No bidding process"}}}},
        {105, {"BidDescriptorType", {}}},
        {125, {"NoUnderlyingSecurityAltID", {}}},
        {145, {"AllocAvgPx", {}}},
        {147, {"AllocNetMoney", {}}},
        {148, {"SettlCurrFxRate", {}}},
        {149, {"SettlCurrFxRateCalc", {{"R","Multiply"},{"D","Divide"}}}},
        {153, {"GrossTradeAmt", {}}},
        {154, {"NoBrokerOfCredit", {}}},
        {155, {"BidForwardPoints2", {}}},
        {156, {"BidForwardPoints", {}}},
        {157, {"NumDaysInterest", {}}},
        {158, {"AccruedInterestRate", {}}},
        {159, {"AccruedInterestAmt", {}}},
        {160, {"SettlInstMode", {}}},
        {162, {"AllocAvgPx", {}}},
        {163, {"AllocNetMoney", {}}},
        {164, {"SettlCurrFxRate", {}}},
        {165, {"SettlCurrFxRateCalc", {}}},
        {166, {"GrossTradeAmt", {}}},
        {168, {"EffectiveTime", {}}},
        {169, {"StandInstDbType", {
            {"0","Other"},{"1","DTC SID"},{"2","Thomson ALERT"},{"3","A Global Custodian"},{"4","AccountNet"}
        }}},
        {170, {"StandInstDbName", {}}},
        {171, {"StandInstDbID", {}}},
        {173, {"SettlInstReqID", {}}},
        {174, {"SettlInstReqRejCode", {}}},
        {175, {"SecondaryAllocID", {}}},
        {176, {"AllocTransType", {}}},
        {177, {"RefAllocID", {}}},
        {178, {"AllocSettlInstType", {
            {"0","Use default instructions"},{"1","Derive from parameters provided"},
            {"2","Full details provided"},{"3","SSI DB IDs provided"},{"4","Phone for instructions"}
        }}},
        {179, {"NoSettlInst", {}}},
        {180, {"AllocAccountID", {}}},
        {181, {"SettlDeliveryType", {}}},
        {182, {"SettlDepositoryCode", {}}},
        {183, {"SettlBrkrCode", {}}},
        {184, {"SettlInstCode", {}}},
        {185, {"SecuritySettlAgentName", {}}},
        {186, {"SecuritySettlAgentCode", {}}},
        {187, {"SecuritySettlAgentAcctNum", {}}},
        {188, {"SecuritySettlAgentAcctName", {}}},
        {189, {"SecuritySettlAgentContactName", {}}},
        {190, {"SecuritySettlAgentContactPhone", {}}},
        {191, {"CashSettlAgentName", {}}},
        {192, {"CashSettlAgentCode", {}}},
        {193, {"FutSettDate2", {}}},
        {194, {"OrderQty2", {}}},
        {195, {"FutSettDate3", {}}},
        {196, {"OrderQty3", {}}},
        {197, {"FutSettDate4", {}}},
        {199, {"OrderQty4", {}}},
        {203, {"CoveredOrUncovered", {{"0","Covered"},{"1","Uncovered"}}}},
        {205, {"MaturityDay", {}}},
        {208, {"NotifyBrokerOfCredit", {{"Y","Yes"},{"N","No"}}}},
        {209, {"AllocHandlInst", {{"1","Match"},{"2","Forward"},{"3","Forward and match"}}}},
        {210, {"MaxShow", {}}},
        {211, {"PegOffsetValue", {}}},
        {214, {"BidTradeType", {{"R","Risk trade"},{"G","VWAP guarantee"},{"A","Agency"},{"J","Guaranteed close"}}}},
        {215, {"BasisPxType", {}}},
        {216, {"NoBidDescriptors", {}}},
        {217, {"BidDescriptorType", {}}},
        {218, {"BidDescriptor", {}}},
        {219, {"SideValueInd", {{"1","Side1"},{"2","Side2"}}}},
        {220, {"LiquidityPctLow", {}}},
        {221, {"LiquidityPctHigh", {}}},
        {222, {"LiquidityValue", {}}},
        {224, {"LiquidityNumSecurities", {}}},
        {225, {"MktImpactPctLow", {}}},
        {226, {"MktImpactPctHigh", {}}},
        {227, {"MktImpactValue", {}}},
        {228, {"BasisPxType", {}}},
        {229, {"BidDescriptor", {}}},
        {230, {"SideValue1", {}}},
        {232, {"NoStipulations", {}}},
        {233, {"StipulationType", {}}},
        {234, {"StipulationValue", {}}},
        {235, {"YieldType", {}}},
        {236, {"Yield", {}}},
        {237, {"TotalTakedown", {}}},
        {238, {"Concession", {}}},
        {239, {"RepoCollateralSecurityType", {}}},
        {240, {"RedemptionDate", {}}},
        {241, {"UnderlyingCouponPaymentDate", {}}},
        {242, {"UnderlyingIssueDate", {}}},
        {243, {"UnderlyingRepoCollateralSecurityType", {}}},
        {244, {"UnderlyingRepoMaturityDate", {}}},
        {245, {"UnderlyingRepoMaturityDate", {}}},
        {246, {"UnderlyingFactor", {}}},
        {247, {"UnderlyingRedemptionDate", {}}},
        {248, {"LegCouponPaymentDate", {}}},
        {249, {"LegIssueDate", {}}},
        {250, {"LegRepoCollateralSecurityType", {}}},
        {251, {"LegRepoMaturityDate", {}}},
        {252, {"LegRepoRate", {}}},
        {253, {"LegFactor", {}}},
        {254, {"LegRedemptionDate", {}}},
        {255, {"CreditRating", {}}},
        {256, {"UnderlyingCreditRating", {}}},
        {257, {"LegCreditRating", {}}},
        {258, {"TradedFlatSwitch", {{"Y","Yes"},{"N","No"}}}},
        {259, {"BasisFeatureDate", {}}},
        {260, {"BasisFeaturePrice", {}}},
        {293, {"TradSesUpdateAction", {{"0","Add"},{"1","Delete"},{"2","Modify"}}}},
        {295, {"NoQuoteEntries", {}}},
        {296, {"NoQuoteSets", {}}},
        {298, {"QuoteAckStatus", {{"0","Accepted"},{"1","Canceled for symbol"},{"2","Canceled for security type"},{"3","Canceled for underlying"},{"4","Canceled all"},{"5","Rejected"}}}},
        {306, {"UnderlyingSecurityDesc", {}}},
        {308, {"UnderlyingSecurityExchange", {}}},
        {309, {"UnderlyingSecurityID", {}}},
        {310, {"UnderlyingIDSource", {}}},
        {311, {"UnderlyingIssuer", {}}},
        {312, {"UnderlyingSecurityType", {}}},
        {313, {"UnderlyingMaturityMonthYear", {}}},
        {315, {"UnderlyingPutOrCall", {{"0","Put"},{"1","Call"}}}},
        {316, {"UnderlyingStrikePrice", {}}},
        {317, {"UnderlyingOptAttribute", {}}},
        {318, {"UnderlyingCurrency", {}}},
        {320, {"SecurityReqID", {}}},
        {321, {"SecurityRequestType", {
            {"0","Request security identity and specifications"},
            {"1","Request security identity for specifications provided"},
            {"2","Request list of securities matching criteria"},{"3","Request list of all securities"}
        }}},
        {322, {"SecurityResponseID", {}}},
        {323, {"SecurityResponseType", {
            {"1","Accept as is"},{"2","Accept with changes"},{"5","Reject security proposal"},{"6","Cannot match selection criteria"}
        }}},
        {324, {"SecurityStatusReqID", {}}},
        {325, {"UnsolicitedIndicator", {{"Y","Yes"},{"N","No"}}}},
        {326, {"SecurityTradingStatus", {
            {"1","Opening delay"},{"2","Trading halt"},{"3","Resume"},{"4","No open / no resume"},
            {"17","Ready to trade"},{"18","Not available for trading"},{"20","Trading resumed"}
        }}},
        {327, {"HaltReasonChar", {{"I","Order imbalance"},{"X","Equipment changeover"},{"P","News pending"},{"D","News dissemination"},{"E","Order influx"},{"M","Additional information"}}}},
        {328, {"InViewOfCommon", {{"Y","Yes"},{"N","No"}}}},
        {329, {"DueToRelated", {{"Y","Yes"},{"N","No"}}}},
        {330, {"BuyVolume", {}}},
        {331, {"SellVolume", {}}},
        {332, {"HighPx", {}}},
        {333, {"LowPx", {}}},
        {334, {"Adjustment", {{"1","Cancel"},{"2","Error"},{"3","Correction"}}}},
        {335, {"TradSesReqID", {}}},
        {338, {"TradSesEvent", {{"0","No event"},{"1","Pre-open"},{"2","Open"},{"3","Disrupted"},{"4","Close"},{"5","New price indication"},{"6","Trade slow"},{"7","Pre-cross"},{"8","Cross"},{"9","New price indication"}}}},
        {339, {"TradSesCloseTime", {}}},
        {340, {"TradSesNewLowLimit", {}}},
        {341, {"TradSesNewHighLimit", {}}},
        {342, {"TradSesPreOpeningTime", {}}},
        {343, {"TradSesCloseTime", {}}},
        {344, {"TradSesNewLowLimit", {}}},
        {345, {"TradSesNewHighLimit", {}}},
        {346, {"NumberOfOrders", {}}},
        {347, {"MessageEncoding", {}}},
        {348, {"EncodedIssuerLen", {}}},
        {349, {"EncodedIssuer", {}}},
        {350, {"EncodedSecurityDescLen", {}}},
        {351, {"EncodedSecurityDesc", {}}},
        {352, {"EncodedListExecInstLen", {}}},
        {353, {"EncodedListExecInst", {}}},
        {354, {"EncodedTextLen", {}}},
        {355, {"EncodedText", {}}},
        {356, {"EncodedSubjectLen", {}}},
        {357, {"EncodedSubject", {}}},
        {358, {"EncodedHeadlineLen", {}}},
        {359, {"EncodedHeadline", {}}},
        {360, {"EncodedAllocTextLen", {}}},
        {361, {"EncodedAllocText", {}}},
        {362, {"EncodedUnderlyingIssuerLen", {}}},
        {364, {"EncodedUnderlyingSecurityDescLen", {}}},
        {365, {"EncodedUnderlyingSecurityDesc", {}}},
        {366, {"AllocPrice", {}}},
        {367, {"QuoteSetValidUntilTime", {}}},
        {368, {"QuoteEntryRejectReason", {}}},
        {370, {"OnBehalfOfSendingTime", {}}},
        {371, {"RefTagID", {}}},
        {374, {"BusinessRejectRefID", {}}},
        {375, {"ContraBroker", {}}},
        {376, {"ComplianceID", {}}},
        {377, {"SolicitedFlag", {{"Y","Yes"},{"N","No"}}}},
        {378, {"ExecRestatementReason", {
            {"0","GT corporate action"},{"1","GT renewal/restatement"},{"2","Verbal change"},
            {"3","Repricing of order"},{"4","Broker option"},{"5","Partial decline of OrderQty"}
        }}},
        {379, {"BusinessRejectRefID", {}}},
        {381, {"GrossTradeAmt", {}}},
        {383, {"MaxMessageSize", {}}},
        {384, {"NoMsgTypes", {}}},
        {385, {"MsgDirection", {{"S","Send"},{"R","Receive"}}}},
        {386, {"NoTradingSessions", {}}},
        {387, {"TotalVolumeTraded", {}}},
        {388, {"DiscretionInst", {}}},
        {389, {"DiscretionOffsetValue", {}}},
        {390, {"BidID", {}}},
        {391, {"ClientBidID", {}}},
        {392, {"ListName", {}}},
        {393, {"TotalNumSecurities", {}}},
        {394, {"BidType", {}}},
        {395, {"NumTickets", {}}},
        {396, {"SideValue1", {}}},
        {397, {"SideValue2", {}}},
        {398, {"NoBidComponents", {}}},
        {399, {"Country", {}}},
        {400, {"TotNoStrikes", {}}},
        {401, {"PriceType", {}}},
        {402, {"DayOrderQty", {}}},
        {403, {"DayCumQty", {}}},
        {404, {"DayAvgPx", {}}},
        {405, {"GTBookingInst", {{"0","Book out all trades"},{"1","Accumulate"},{"2","Accumulate until verbally notified"}}}},
        {406, {"NoStrikes", {}}},
        {407, {"ListStatusType", {{"1","Ack"},{"2","Response"},{"3","Timed"},{"4","Exec started"},{"5","All done"},{"6","Alert"}}}},
        {408, {"NetGrossInd", {{"1","Net"},{"2","Gross"}}}},
        {409, {"ListOrderStatus", {{"1","In biddable process"},{"2","Received for execution"},{"3","Executing"},{"4","Canceling"},{"5","Alert"},{"6","All done"},{"7","Reject"}}}},
        {410, {"NoRpts", {}}},
        {411, {"RptSeq", {}}},
        {412, {"ListOrderStatus", {}}},
        {413, {"ListExecInstType", {}}},
        {414, {"CxlRejResponseTo", {}}},
        {415, {"UnderlyingCouponRate", {}}},
        {416, {"UnderlyingContractMultiplier", {}}},
        {417, {"ContraTradeQty", {}}},
        {418, {"ContraTradeTime", {}}},
        {419, {"LiquidityNumSecurities", {}}},
        {420, {"MultiLegReportingType", {}}},
        {421, {"StrikeTime", {}}},
        {422, {"ListStatusText", {}}},
        {424, {"DayOrderQty", {}}},
        {425, {"DayCumQty", {}}},
        {426, {"DayAvgPx", {}}},
        {427, {"GTBookingInst", {}}},
        {428, {"NoStrikes", {}}},
        {429, {"ListStatusType", {}}},
        {430, {"NetGrossInd", {}}},
        {431, {"ListOrderStatus", {}}},
        {433, {"ListExecInstType", {}}},
        {435, {"UnderlyingCouponRate", {}}},
        {436, {"UnderlyingContractMultiplier", {}}},
        {437, {"ContraTradeQty", {}}},
        {438, {"ContraTradeTime", {}}},
        {439, {"ClearingFirm", {}}},
        {440, {"ClearingAccount", {}}},
        {441, {"LiquidityNumSecurities", {}}},
        {443, {"MultiLegReportingType", {}}},
        {444, {"StrikeTime", {}}},
        {445, {"ListStatusText", {}}},
        {446, {"EncodedListStatusTextLen", {}}},
        {449, {"PartySubID", {}}},
        {450, {"NetChgPrevDay", {}}},
        {451, {"NetChgPrevDay", {}}},
        {455, {"SecurityAltID", {}}},
        {456, {"SecurityAltIDSource", {}}},
        {457, {"UnderlyingSecurityAltID", {}}},
        {458, {"UnderlyingSecurityAltIDSource", {}}},
        {459, {"NoUnderlyingSecurityAltID", {}}},
        {463, {"UnderlyingProduct", {}}},
        {464, {"TestMessageIndicator", {{"Y","Yes"},{"N","No"}}}},
        {465, {"QuantityType", {}}},
        {466, {"BookingRefID", {}}},
        {467, {"IndividualAllocID", {}}},
        {468, {"RoundingDirection", {{"0","Round to nearest"},{"1","Round down"},{"2","Round up"}}}},
        {469, {"RoundingModulus", {}}},
        {470, {"CountryOfIssue", {}}},
        {471, {"StateOrProvinceOfIssue", {}}},
        {472, {"LocaleOfIssue", {}}},
        {473, {"NoRegistDtls", {}}},
        {474, {"MailingDtls", {}}},
        {475, {"InvestorCountryOfResidence", {}}},
        {476, {"PaymentRef", {}}},
        {477, {"DistribPaymentMethod", {}}},
        {478, {"CashDistribCurr", {}}},
        {479, {"CommCurrency", {}}},
        {480, {"CancellationRights", {{"Y","Yes"},{"N","No"},{"N/A","Not applicable"}}}},
        {481, {"MoneyLaunderingStatus", {{"Y","Passed"},{"N","Not checked"},{"1","Exempt residence"},{"2","Exempt domicile"},{"3","Exempt beneficiary"}}}},
        {482, {"MailingInst", {}}},
        {483, {"TransBkdTime", {}}},
        {484, {"ExecPriceType", {}}},
        {485, {"ExecPriceAdjustment", {}}},
        {486, {"DateOfBirth", {}}},
        {488, {"ExecValuationPoint", {}}},
        {489, {"OrderPercent", {}}},
        {490, {"NoCashDistribInstructions", {}}},
        {491, {"CashDistribAgentName", {}}},
        {492, {"CashDistribAgentCode", {}}},
        {493, {"CashDistribAgentAcctNumber", {}}},
        {494, {"CashDistribPayRef", {}}},
        {495, {"CashDistribAgentAcctName", {}}},
        {496, {"CardHolderName", {}}},
        {497, {"CardNumber", {}}},
        {498, {"CardExpDate", {}}},
        {499, {"CardIssNum", {}}},
        {500, {"PaymentMethod", {{"1","Crossed check"},{"2","Bankers draft"},{"3","Telegraphic transfer"},{"4","Electronic transfer"},{"5","Direct debit"}}}},
        {501, {"RegAcctType", {}}},
        {502, {"Designation", {}}},
        {503, {"TaxAdvantageType", {}}},
        {504, {"RegistAcctType", {}}},
        {505, {"FundRenewWaiv", {}}},
        {506, {"CashDistribAgentAddress", {}}},
        {507, {"NoContAmts", {}}},
        {508, {"ContAmtType", {}}},
        {509, {"ContAmtValue", {}}},
        {510, {"ContAmtCurr", {}}},
        {511, {"OwnerType", {}}},
        {512, {"PartyID", {}}},
        {513, {"NoRegistDtls", {}}},
        {514, {"RegistDtls", {}}},
        {515, {"RegistEmail", {}}},
        {516, {"MailingDtls", {}}},
        {517, {"MailingInst", {}}},
        {518, {"RegistID", {}}},
        {519, {"RegistRejReasonText", {}}},
        {520, {"FundRenewWaiv", {}}},
        {521, {"RegistRejReasonCode", {}}},
        {522, {"RegistRefID", {}}},
        {523, {"RegistDetls", {}}},
        {524, {"NoDistribInsts", {}}},
        {525, {"RegistEmail", {}}},
        {526, {"SecondaryClOrdID", {}}},
        {530, {"MassCancelRequestType", {
            {"1","Cancel orders for a security"},{"2","Cancel orders for an underlying security"},
            {"3","Cancel orders for a product"},{"4","Cancel orders for a security type"},
            {"5","Cancel orders for a trading session"},{"6","Cancel all orders"}
        }}},
        {531, {"MassCancelResponse", {}}},
        {532, {"MassCancelRejectReason", {}}},
        {533, {"TotalAffectedOrders", {}}},
        {534, {"NoAffectedOrders", {}}},
        {535, {"AffectedOrderID", {}}},
        {536, {"AffectedSecondaryOrderID", {}}},
        {537, {"QuoteType", {{"0","Indicative"},{"1","Tradeable"},{"2","Restricted tradeable"},{"3","Counter"}}}},
        {538, {"LegPositionEffect", {}}},
        {539, {"NoContraBrokers", {}}},
        {540, {"ContraTradeQty", {}}},
        {541, {"ContraTradeTime", {}}},
        {542, {"LegMaturityDate", {}}},
        {543, {"LegStrikePrice", {}}},
        {544, {"LegOptAttribute", {}}},
        {545, {"LegContractMultiplier", {}}},
        {546, {"LegCouponRate", {}}},
        {547, {"LegSecurityExchange", {}}},
        {548, {"LegIssuer", {}}},
        {549, {"LegSecurityDesc", {}}},
        {550, {"LegSecurityType", {}}},
        {551, {"LegSecurityID", {}}},
        {553, {"Username", {}}},
        {554, {"Password", {}}},
        {557, {"NoStrikeRules", {}}},
        {558, {"StrikeRuleID", {}}},
        {559, {"ExchangeForPhysical", {{"Y","Yes"},{"N","No"}}}},
        {560, {"NoLegSecurityAltID", {}}},
        {561, {"RoundLot", {}}},
        {562, {"MinTradeVol", {}}},
        {563, {"LegPositionEffect", {}}},
        {564, {"LegCoveredOrUncovered", {}}},
        {565, {"LegSettlmntTyp", {}}},
        {566, {"LegFutSettDate", {}}},
        {567, {"QuoteRejectReason", {
            {"1","Unknown symbol"},{"2","Exchange closed"},{"3","Quote exceeds limit"},{"4","Too late to enter"},
            {"5","Unknown quote"},{"6","Duplicate quote"},{"7","Invalid bid/ask spread"},{"8","Invalid price"}
        }}},
        {568, {"QuoteResponseID", {}}},
        {569, {"QuoteRequestType", {}}},
        {570, {"PreviouslyReported", {{"Y","Yes"},{"N","No"}}}},
        {571, {"TradeReportID", {}}},
        {572, {"TradeReportRefID", {}}},
        {573, {"MatchStatus", {{"0","Compared"},{"1","Uncompared"},{"2","Advisory or alert"}}}},
        {574, {"MatchType", {}}},
        {575, {"OddLot", {{"Y","Yes"},{"N","No"}}}},
        {576, {"NoClearingInstructions", {}}},
        {577, {"ClearingInstruction", {}}},
        {578, {"TradeInputSource", {}}},
        {579, {"TradeInputDevice", {}}},
        {580, {"NoDates", {}}},
        {582, {"CustOrderCapacity", {
            {"1","Member trading for their own account"},{"2","Clearing firm trading for its own account"},
            {"3","Member trading for another member"},{"4","All other"}
        }}},
        {583, {"ClOrdLinkID", {}}},
        {586, {"OrigOrdModTime", {}}},
        {587, {"LegSettlType", {}}},
        {588, {"LegSettlDate", {}}},
        {589, {"DayBookingInst", {}}},
        {590, {"BookingUnit", {}}},
        {591, {"PreallocMethod", {{"0","Pro-rata"},{"1","Do not pro-rata"}}}},
        {592, {"UnderlyingCountryOfIssue", {}}},
        {593, {"UnderlyingStateOrProvinceOfIssue", {}}},
        {594, {"UnderlyingLocaleOfIssue", {}}},
        {595, {"UnderlyingInstrRegistry", {}}},
        {596, {"LegCountryOfIssue", {}}},
        {597, {"LegStateOrProvinceOfIssue", {}}},
        {598, {"LegLocaleOfIssue", {}}},
        {599, {"LegInstrRegistry", {}}},
        {600, {"LegSymbol", {}}},
        {601, {"LegSymbolSfx", {}}},
        {602, {"LegSecurityID", {}}},
        {603, {"LegIDSource", {}}},
        {604, {"NoLegSecurityAltID", {}}},
        {605, {"LegSecurityAltID", {}}},
        {606, {"LegSecurityAltIDSource", {}}},
        {607, {"LegRatioQty", {}}},
        {609, {"LegSecurityType", {}}},
        {610, {"LegMaturityMonthYear", {}}},
        {611, {"LegMaturityDate", {}}},
        {612, {"LegStrikePrice", {}}},
        {613, {"LegOptAttribute", {}}},
        {614, {"LegContractMultiplier", {}}},
        {615, {"LegCouponRate", {}}},
        {616, {"LegSecurityExchange", {}}},
        {617, {"LegIssuer", {}}},
        {618, {"EncodedLegIssuerLen", {}}},
        {619, {"EncodedLegIssuer", {}}},
        {620, {"LegSecurityDesc", {}}},
        {621, {"EncodedLegSecurityDescLen", {}}},
        {622, {"EncodedLegSecurityDesc", {}}},
        {623, {"LegRatioQty", {}}},
        {625, {"TradingSessionSubID", {}}},
        {626, {"AllocType", {}}},
        {627, {"NoHops", {}}},
        {628, {"HopCompID", {}}},
        {629, {"HopSendingTime", {}}},
        {630, {"HopRefID", {}}},
        {631, {"MidPx", {}}},
        {632, {"BidYield", {}}},
        {633, {"MidYield", {}}},
        {634, {"OfferYield", {}}},
        {635, {"ClearingFeeIndicator", {}}},
        {637, {"LegLastPx", {}}},
        {638, {"PriorityIndicator", {{"0","Priority unchanged"},{"1","Priority increased"}}}},
        {639, {"PriceImprovement", {}}},
        {640, {"PreviousClosePx", {}}},
        {654, {"LegRefID", {}}},
        {655, {"ContraLegRefID", {}}},
        {656, {"SettlCurrBidFxRate", {}}},
        {657, {"SettlCurrOfferFxRate", {}}},
        {658, {"QuoteRequestRejectReason", {
            {"1","Unknown symbol"},{"2","Exchange closed"},{"3","Quote request exceeds limit"},
            {"4","Too late to enter"},{"5","Invalid price"},{"6","Not authorized to request quote"}
        }}},
        {659, {"SideComplianceID", {}}},
        {660, {"AcctIDSource", {}}},
        {661, {"AllocAcctIDSource", {}}},
        {662, {"BenchmarkPrice", {}}},
        {664, {"NoNested2PartyIDs", {}}},
        {665, {"TotNoAllocs", {}}},
        {666, {"LastFragment", {{"Y","Yes"},{"N","No"}}}},
        {667, {"CollReqID", {}}},
        {668, {"CollAsgnReason", {}}},
        {669, {"CollInquiryQualifier", {}}},
        {670, {"NoTrades", {}}},
        {671, {"LegReportID", {}}},
        {672, {"SettlInstID", {}}},
        {673, {"SettlInstRefID", {}}},
        {674, {"NoSettlPartyIDs", {}}},
        {675, {"SettlPartyID", {}}},
        {676, {"SettlPartyIDSource", {}}},
        {677, {"SettlPartyRole", {}}},
        {678, {"NoUnderlyingAmounts", {}}},
        {679, {"UnderlyingPayAmount", {}}},
        {680, {"UnderlyingCollectAmount", {}}},
        {681, {"UnderlyingSettlementValue", {}}},
        {683, {"NoLegStipulations", {}}},
        {684, {"SettlInstReqID", {}}},
        {685, {"LegStipulationType", {}}},
        {686, {"LegStipulationValue", {}}},
        {687, {"LegQty", {}}},
        {688, {"LegSwapType", {}}},
        {689, {"Pool", {}}},
        {690, {"QuotePriceType", {}}},
        {691, {"QuoteRespID", {}}},
        {692, {"QuoteRespType", {{"1","Hit/Lift"},{"2","Counter"},{"3","Expired"},{"4","Cover"},{"5","Done away"},{"6","Pass"}}}},
        {693, {"QuoteQualifier", {}}},
        {694, {"YieldRedemptionDate", {}}},
        {695, {"YieldRedemptionPrice", {}}},
        {696, {"YieldRedemptionPriceType", {}}},
        {697, {"BenchmarkSecurityID", {}}},
        {698, {"ReversalIndicator", {{"Y","Yes"},{"N","No"}}}},
        {699, {"YieldCalcDate", {}}},
        {700, {"NoPositions", {}}},
        {701, {"PosType", {}}},
        {702, {"LongQty", {}}},
        {703, {"ShortQty", {}}},
        {704, {"PosQtyStatus", {{"0","Submitted"},{"1","Accepted"},{"2","Rejected"}}}},
        {705, {"PosAmtType", {}}},
        {706, {"PosAmt", {}}},
        {707, {"PosTransType", {}}},
        {708, {"PosReqID", {}}},
        {709, {"NoUnderlyings", {}}},
        {710, {"PosMaintRptRefID", {}}},
        {712, {"PosMaintAction", {{"1","New"},{"2","Replace"},{"3","Cancel"},{"4","Reverse"}}}},
        {713, {"OrigPosReqRefID", {}}},
        {714, {"PosMaintRptID", {}}},
        {715, {"ClearingBusinessDate", {}}},
        {716, {"SettlSessID", {}}},
        {717, {"SettlSessSubID", {}}},
        {718, {"AdjustmentType", {{"0","Process request as margin disposition"},{"1","Delta plus"},{"2","Delta minus"},{"3","Final"}}}},
        {719, {"ContraryInstructionIndicator", {{"Y","Yes"},{"N","No"}}}},
        {720, {"PriorSpreadIndicator", {{"Y","Yes"},{"N","No"}}}},
        {721, {"PosMaintRptID", {}}},
        {722, {"PosMaintStatus", {{"0","Accepted"},{"1","Accepted with warnings"},{"2","Rejected"},{"3","Completed"}}}},
        {723, {"PosMaintResult", {}}},
        {724, {"PosReqType", {
            {"0","Positions"},{"1","Trades"},{"2","Exercises"},{"3","Assignments"},{"4","Settlement activity"}
        }}},
        {725, {"ResponseTransportType", {{"0","Inband"},{"1","Out-of-band"}}}},
        {726, {"ResponseDestination", {}}},
        {727, {"TotalNumPosReports", {}}},
        {728, {"PosReqResult", {
            {"0","Valid request"},{"1","Invalid/unsupported request"},{"2","No positions found"},
            {"3","Not authorized"},{"4","Request for position not supported"}
        }}},
        {729, {"PosReqStatus", {{"0","Completed"},{"1","Completed with warnings"},{"2","Rejected"}}}},
        {730, {"SettlPrice", {}}},
        {731, {"SettlPriceType", {{"1","Final"},{"2","Theoretical"}}}},
        {732, {"UnderlyingSettlPrice", {}}},
        {733, {"UnderlyingSettlPriceType", {}}},
        {734, {"PriorSettlPrice", {}}},
        {735, {"NoQuoteQualifiers", {}}},
        {736, {"AllocSettlCurrency", {}}},
        {737, {"AllocSettlCurrAmt", {}}},
        {738, {"InterestAtMaturity", {}}},
        {739, {"LegDatedDate", {}}},
        {740, {"LegPool", {}}},
        {741, {"AllocInterestAtMaturity", {}}},
        {742, {"AllocAccruedInterestAmt", {}}},
        {743, {"DeliveryDate", {}}},
        {744, {"AssignmentMethod", {{"P","Pro-rata"},{"R","Random"}}}},
        {745, {"AssignmentUnit", {}}},
        {746, {"OpenInterest", {}}},
        {747, {"ExerciseMethod", {{"A","Automatic"},{"M","Manual"}}}},
        {748, {"TotNumTradeReports", {}}},
        {749, {"TradeRequestResult", {
            {"0","Successful"},{"1","Invalid/unsupported request"},{"2","No matches"},{"3","Not authorized"}
        }}},
        {750, {"TradeRequestStatus", {{"0","Accepted"},{"1","Completed"},{"2","Rejected"}}}},
        {751, {"TradeReportRejectReason", {}}},
        {752, {"SideMultiLegReportingType", {}}},
        {753, {"NoPosAmt", {}}},
        {754, {"AutoAcceptIndicator", {{"Y","Yes"},{"N","No"}}}},
        {755, {"AllocReportID", {}}},
        {756, {"NoNested2PartyIDs", {}}},
        {757, {"Nested2PartyID", {}}},
        {758, {"Nested2PartyIDSource", {}}},
        {759, {"Nested2PartyRole", {}}},
        {760, {"Nested2PartySubID", {}}},
        {761, {"BenchmarkSecurityIDSource", {}}},
        {762, {"SecuritySubType", {}}},
        {763, {"UnderlyingSecuritySubType", {}}},
        {764, {"LegSecuritySubType", {}}},
        {765, {"AllowableOneSidednessPct", {}}},
        {766, {"AllowableOneSidednessValue", {}}},
        {767, {"AllowableOneSidednessCurr", {}}},
        {768, {"NoTrdRegTimestamps", {}}},
        {769, {"TrdRegTimestamp", {}}},
        {770, {"TrdRegTimestampType", {}}},
        {771, {"TrdRegTimestampOrigin", {}}},
        {772, {"ConfirmRefID", {}}},
        {773, {"ConfirmType", {{"1","Status"},{"2","Confirmation"},{"3","Confirmation request rejected"}}}},
        {774, {"ConfirmRejReason", {}}},
        {775, {"BookingType", {{"0","Regular booking"},{"1","CFD"},{"2","Total return swap"}}}},
        {776, {"IndividualAllocRejCode", {}}},
        {777, {"SettlInstMsgID", {}}},
        {778, {"NoSettlInst", {}}},
        {779, {"LastUpdateTime", {}}},
        {780, {"AllocSettlInstType", {}}},
        {781, {"NoSettlPartyIDs", {}}},
        {782, {"SettlPartyID", {}}},
        {783, {"SettlPartyIDSource", {}}},
        {784, {"SettlPartyRole", {}}},
        {785, {"SettlPartySubID", {}}},
        {786, {"SettlPartySubIDType", {}}},
        {787, {"DlvyInstType", {{"S","Security"},{"C","Cash"}}}},
        {788, {"TerminationType", {{"1","Overnight"},{"2","Term"},{"3","Flexible"},{"4","Open"}}}},
        {789, {"NextExpectedMsgSeqNum", {}}},
        {790, {"OrdStatusReqID", {}}},
        {791, {"SettlInstReqID", {}}},
        {792, {"AvgPxIndicator", {{"0","No average pricing"},{"1","Trade is part of an average price group"},{"2","Last trade for the average price group"}}}},
        {793, {"TradeLinkID", {}}},
        {794, {"OrderInputDevice", {}}},
        {795, {"UnderlyingTradingSessionID", {}}},
        {796, {"UnderlyingTradingSessionSubID", {}}},
        {798, {"AllocNoOrdersType", {{"0","Not specified"},{"1","Explicit list provided"}}}},
        {799, {"OrderAvgPx", {}}},
        {800, {"OrderBookingQty", {}}},
        {801, {"NoSettlPartySubIDs", {}}},
        {802, {"NoPartySubIDs", {}}},
        {803, {"PartySubIDType", {}}},
        {804, {"NoNestedPartySubIDs", {}}},
        {805, {"NestedPartySubIDType", {}}},
        {806, {"NoNested2PartySubIDs", {}}},
        {807, {"Nested2PartySubIDType", {}}},
        {808, {"AllocIntermedReqType", {{"1","Pending accept"},{"2","Pending release"},{"3","Pending reversal"},{"4","Accept"},{"5","Block level reject"},{"6","Account level reject"}}}},
        {809, {"UnderlyingPx", {}}},
        {810, {"PriceDelta", {}}},
        {811, {"ApplQueueMax", {}}},
        {812, {"ApplQueueDepth", {}}},
        {813, {"ApplQueueResolution", {}}},
        {814, {"ApplQueueAction", {}}},
        {815, {"NoAltMDSource", {}}},
        {816, {"AltMDSourceID", {}}},
        {817, {"SecondaryTradeReportID", {}}},
        {818, {"AvgPxIndicator", {}}},
        {819, {"TradeLinkID", {}}},
        {820, {"OrderInputDevice", {}}},
        {821, {"UnderlyingTradingSessionID", {}}},
        {822, {"UnderlyingTradingSessionSubID", {}}},
        {823, {"TradeLegRefID", {}}},
        {824, {"ExchangeRule", {}}},
        {825, {"TradeAllocIndicator", {{"0","Allocation not required"},{"1","Allocation required"},{"2","Use pre-allocation"},{"3","Real time execution allocation required"}}}},
        {826, {"ExpirationCycle", {{"0","Expire on trading session close"},{"1","Expire on trading session open"}}}},
        {827, {"ExpirationQty", {}}},
        {828, {"TrdType", {
            {"0","Regular trade"},{"1","Block trade"},{"2","EFP"},{"3","Transfer"},
            {"4","Late trade"},{"5","T trade"},{"8","Privately negotiated trade"}
        }}},
        {829, {"TrdSubType", {}}},
        {830, {"TransferReason", {}}},
        {831, {"AsgnReqID", {}}},
        {832, {"TotNumAssignmentReports", {}}},
        {833, {"AsgnRptID", {}}},
        {834, {"ThresholdAmount", {}}},
        {835, {"PegMoveType", {{"0","Floating"},{"1","Fixed"}}}},
        {836, {"PegOffsetType", {{"0","Price"},{"1","Basis points"},{"2","Ticks"},{"3","Price tier/level"}}}},
        {837, {"PegLimitType", {{"0","Or better"},{"1","Strict"},{"2","Or worse"}}}},
        {838, {"PegRoundDirection", {{"1","More aggressive"},{"2","More passive"}}}},
        {839, {"PeggedPrice", {}}},
        {840, {"PegScope", {{"1","Local/Exchange"},{"2","National"},{"3","Global"},{"4","National excluding local"}}}},
        {841, {"DiscretionMoveType", {{"0","Floating"},{"1","Fixed"}}}},
        {842, {"DiscretionOffsetType", {}}},
        {843, {"DiscretionLimitType", {}}},
        {844, {"DiscretionRoundDirection", {}}},
        {845, {"DiscretionPrice", {}}},
        {846, {"DiscretionScope", {}}},
        {847, {"TargetStrategy", {{"1","VWAP"},{"2","Participate"},{"3","Mininize Market Impact"}}}},
        {848, {"TargetStrategyParameters", {}}},
        {849, {"ParticipationRate", {}}},
        {850, {"TargetStrategyPerformance", {}}},
        {851, {"LastLiquidityInd", {{"1","Added liquidity"},{"2","Removed liquidity"},{"3","Liquidity routed out"},{"4","Auction"}}}},
        {852, {"PublishTrdIndicator", {{"Y","Yes"},{"N","No"}}}},
        {853, {"ShortSaleReason", {}}},
        {854, {"QtyType", {{"0","Units"},{"1","Contracts"}}}},
        {855, {"SecondaryTrdType", {}}},
        {856, {"TradeReportType", {
            {"0","Submit"},{"1","Alleged"},{"2","Accept"},{"3","Decline"},{"4","Addendum"},
            {"5","No/was"},{"6","Trade report cancel"},{"7","Locked-in trade break"}
        }}},
        {857, {"AllocNoOrdersType", {}}},
        {858, {"SharedCommission", {}}},
        {859, {"ConfirmReqID", {}}},
        {860, {"AvgParPx", {}}},
        {861, {"ReportedPx", {}}},
        {862, {"NoCapacities", {}}},
        {863, {"OrderCapacityQty", {}}},
        {864, {"NoEvents", {}}},
        {865, {"EventType", {{"1","Put"},{"2","Call"},{"3","Tender"},{"4","Sinking fund call"},{"5","Activation"},{"6","Inactivation"},{"7","Last eligible trade date"}}}},
        {866, {"EventDate", {}}},
        {867, {"EventPx", {}}},
        {868, {"EventText", {}}},
        {869, {"PctAtRisk", {}}},
        {870, {"NoInstrAttrib", {}}},
        {871, {"InstrAttribType", {}}},
        {872, {"InstrAttribValue", {}}},
        {873, {"DatedDate", {}}},
        {874, {"InterestAccrualDate", {}}},
        {875, {"CPProgram", {{"1","3(a)(3)"},{"2","4(2)"}}}},
        {876, {"CPRegType", {}}},
        {877, {"UnderlyingCPProgram", {}}},
        {878, {"UnderlyingCPRegType", {}}},
        {879, {"UnderlyingQty", {}}},
        {880, {"TrdMatchID", {}}},
        {881, {"SecondaryTradeReportRefID", {}}},
        {882, {"UnderlyingDirtyPrice", {}}},
        {883, {"UnderlyingEndPrice", {}}},
        {884, {"UnderlyingStartValue", {}}},
        {885, {"UnderlyingCurrentValue", {}}},
        {886, {"UnderlyingEndValue", {}}},
        {887, {"NoUnderlyingStips", {}}},
        {888, {"UnderlyingStipType", {}}},
        {889, {"UnderlyingStipValue", {}}},
        {890, {"MaturityNetMoney", {}}},
        {891, {"MiscFeeBasis", {{"0","Absolute"},{"1","Per unit"},{"2","Percentage"}}}},
        {892, {"TotNoAllocs", {}}},
        {894, {"CollAsgnReason", {
            {"0","Initial"},{"1","Scheduled"},{"2","Time warning"},{"3","Margin deficiency"},
            {"4","Margin excess"},{"5","Forward collateral demand"},{"6","Event of default"},{"7","Adverse currency"}
        }}},
        {895, {"CollInquiryQualifier", {}}},
        {896, {"NoTrades", {}}},
        {897, {"MarginExcess", {}}},
        {898, {"TotalNetValue", {}}},
        {899, {"CashOutstanding", {}}},
        {900, {"CollAsgnID", {}}},
        {901, {"CollAsgnTransType", {{"0","New"},{"1","Replace"},{"2","Cancel"},{"3","Release"},{"4","Reverse"}}}},
        {902, {"CollRespID", {}}},
        {903, {"CollAsgnRespType", {{"0","Received"},{"1","Accepted"},{"2","Declined"},{"3","Rejected"}}}},
        {904, {"CollAsgnRejectReason", {}}},
        {905, {"CollAsgnRefID", {}}},
        {906, {"CollRptID", {}}},
        {907, {"CollInquiryID", {}}},
        {908, {"CollStatus", {{"0","Unassigned"},{"1","Partially assigned"},{"2","Assignment proposed"},{"3","Assigned"},{"4","Challenged"}}}},
        {909, {"TotNumReports", {}}},
        {910, {"LastRptRequested", {{"Y","Yes"},{"N","No"}}}},
        {911, {"AgreementDesc", {}}},
        {912, {"AgreementID", {}}},
        {913, {"AgreementDate", {}}},
        {914, {"StartDate", {}}},
        {915, {"EndDate", {}}},
        {916, {"AgreementCurrency", {}}},
        {917, {"DeliveryType", {{"0","Versus payment"},{"1","Free"},{"2","Tri-party"},{"3","Hold in custody"}}}},
        {918, {"EndAccruedInterestAmt", {}}},
        {919, {"StartCash", {}}},
        {920, {"EndCash", {}}},
        {921, {"UserRequestID", {}}},
        {922, {"UserRequestType", {{"1","Logon"},{"2","Logoff"},{"3","Change password"},{"4","Request status"}}}},
        {923, {"NewPassword", {}}},
        {924, {"UserStatus", {{"1","Logged in"},{"2","Not logged in"},{"3","User not recognized"},{"4","Password incorrect"},{"5","Password changed"}}}},
        {926, {"UserStatusText", {}}},
        {927, {"StatusValue", {}}},
        {928, {"StatusText", {}}},
        {929, {"RefCompID", {}}},
        {930, {"RefSubID", {}}},
        {931, {"NetworkResponseID", {}}},
        {932, {"NetworkRequestID", {}}},
        {933, {"LastNetworkResponseID", {}}},
        {934, {"NetworkRequestType", {}}},
        {935, {"NoCompIDs", {}}},
        {936, {"NetworkStatusResponseType", {}}},
        {937, {"NoCollInquiryQualifier", {}}},
        {938, {"TotNumTradeReports", {}}},
        {939, {"TradeRequestResult", {}}},
        {940, {"TradeRequestStatus", {}}},
        {941, {"TradeReportRejectReason", {}}},
        {942, {"SideMultiLegReportingType", {}}},
        {943, {"TimeBracket", {}}},
        {944, {"CollAction", {{"0","Retain"},{"1","Add"},{"2","Remove"}}}},
        {945, {"CollInquiryStatus", {}}},
        {946, {"CollInquiryResult", {}}},
        {947, {"DayBookingInst", {}}},
        {948, {"BookingUnit", {}}},
        {949, {"PreallocMethod", {}}},
        {950, {"UnderlyingCountryOfIssue", {}}},
        {952, {"AccruedInterestAmt", {}}},
        {953, {"NoSecurityAltID", {}}},
        {954, {"NoLegSecurityAltID", {}}},
        {955, {"LegSwapType", {}}},
        {956, {"Pool", {}}},
        {957, {"QuotePriceType", {}}},
        {958, {"QuoteRespID", {}}},
        {959, {"QuoteRespType", {}}},
        {960, {"QuoteQualifier", {}}},
        {961, {"YieldRedemptionDate", {}}},
        {962, {"YieldRedemptionPrice", {}}},

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
    // Find the position of the "10=" tag (checksum field) — sum everything before it.
    size_t checksumTagPos = std::string::npos;
    for (size_t i = 0; i + 3 <= msg.size(); ++i) {
        if (msg[i] == '1' && msg[i+1] == '0' && msg[i+2] == '=') {
            if (i == 0 || msg[i-1] == delim) {
                checksumTagPos = i;
            }
        }
    }
    size_t sumUpTo = (checksumTagPos != std::string::npos) ? checksumTagPos : msg.size();

    // FIX spec section 4.3: the checksum is computed over the raw on-wire bytes
    // where every field separator is SOH (ASCII 0x01, value 1).  When a user
    // pastes a message using a display delimiter ('|', '^', ';' etc.) we must
    // treat those delimiter characters as value 1 for the sum — otherwise a '|'
    // (value 124) contributes 123 extra per field and the calculated checksum
    // will never match the embedded one.
    const bool normaliseDelim = (delim != '\x01');
    unsigned int sum = 0;
    for (size_t i = 0; i < sumUpTo; ++i) {
        unsigned char byte = static_cast<unsigned char>(msg[i]);
        if (normaliseDelim && msg[i] == delim) {
            byte = 1; // treat as SOH
        }
        sum += byte;
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

// ---------- Reusable single-message parser ----------
// Used by both /api/parse (single message) and /api/parse-log (multi-message log).
// Maps a FIX BeginString (e.g. "FIX.4.4") to the OnixS dictionary's URL version segment.
// Falls back to 4.4 (the most common production version) if we don't recognize the string,
// so the link is still useful even if slightly off-version.
static std::string mapBeginStringToOnixsVersion(const std::string& beginString) {
    if (beginString == "FIX.4.0") return "4.0";
    if (beginString == "FIX.4.1") return "4.1";
    if (beginString == "FIX.4.2") return "4.2";
    if (beginString == "FIX.4.3") return "4.3";
    if (beginString == "FIX.4.4") return "4.4";
    if (beginString == "FIXT.1.1") return "5.0.SP2"; // FIX 5.0+ uses FIXT session layer
    if (beginString.find("5.0") != std::string::npos) return "5.0.SP2";
    return "4.4";
}

static std::string buildOnixsReferenceUrl(int tag, const std::string& onixsVersion) {
    return "https://www.onixs.biz/fix-dictionary/" + onixsVersion + "/tagNum_" + std::to_string(tag) + ".html";
}

static crow::json::wvalue parseOneMessage(const std::string& body) {
    crow::json::wvalue result;

    if (body.empty()) {
        result["status"] = "error";
        result["message"] = "Empty input";
        return result;
    }

    char delim = detectDelimiter(body);
    std::vector<FixToken> tokens = tokenize(body, delim);

    const auto& dict = getDictionary();
    const auto& groupStarts = getGroupStartTags();

    result["status"] = "ok";

    std::vector<crow::json::wvalue> headerArr;
    std::vector<crow::json::wvalue> bodyArr;
    std::vector<crow::json::wvalue> trailerArr;
    std::vector<crow::json::wvalue> sequence;

    std::string msgType = "";
    std::string senderCompID = "";
    std::string targetCompID = "";
    std::string sendingTime = "";
    std::string clOrdID = "";
    std::string origClOrdID = "";

    // Build reverse lookup: group-start-tag -> the counter tag that activates it.
    // (Multiple counters could theoretically map to the same start tag, e.g. 267/268
    // both map to 269 — we just need to know if ANY relevant counter is present.)
    std::unordered_map<int, std::vector<int>> startTagToCounters;
    for (auto& kv : groupStarts) {
        startTagToCounters[kv.second].push_back(kv.first);
    }

    // Determine which counter tags actually appear in THIS message — only these
    // activate repeating-group detection. This prevents false positives where a
    // tag number happens to match a "group start" value (e.g. tag 54/Side is the
    // group-start for NoSides, but a plain order's Side field is not a repeating
    // group instance unless NoSides/552 is actually present).
    std::unordered_set<int> activeCounterTags;
    for (const auto& tok : tokens) {
        if (groupStarts.count(tok.tag) > 0) {
            activeCounterTags.insert(tok.tag);
        }
    }

    // A start tag is "live" only if at least one of its associated counters is
    // actually present in this message.
    std::unordered_set<int> liveGroupStartTags;
    for (auto& kv : startTagToCounters) {
        int startTag = kv.first;
        for (int counterTag : kv.second) {
            if (activeCounterTags.count(counterTag) > 0) {
                liveGroupStartTags.insert(startTag);
                break;
            }
        }
    }

    int currentGroupIndex = -1;
    bool anyCounterSeenSoFar = false;

    // Pre-scan for BeginString (tag 8) so we know which FIX version to link to
    // for "learn more" / unknown-tag reference URLs.
    std::string beginString = "";
    for (const auto& tok : tokens) {
        if (tok.tag == 8) {
            beginString = std::string(tok.value);
            break;
        }
    }
    std::string onixsVersion = mapBeginStringToOnixsVersion(beginString);

    for (size_t idx = 0; idx < tokens.size(); ++idx) {
        const auto& tok = tokens[idx];
        crow::json::wvalue entry;
        entry["tag"] = tok.tag;

        std::string rawVal(tok.value);
        entry["raw"] = rawVal;

        auto it = dict.find(tok.tag);
        std::string fieldName;
        bool isUnknownTag = false;
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
            isUnknownTag = true;
        }
        entry["isUnknownTag"] = isUnknownTag;
        entry["referenceUrl"] = buildOnixsReferenceUrl(tok.tag, onixsVersion);

        const auto& whyMap = getWhyExplanations();
        auto whyIt = whyMap.find(tok.tag);
        if (whyIt != whyMap.end()) {
            entry["why"] = whyIt->second;
        } else {
            entry["why"] = "Part of this message's data — not one of the most commonly explained fields, but still required by the FIX dictionary for this tag.";
        }

        bool isGroupCounter = groupStarts.count(tok.tag) > 0;
        entry["isGroupCounter"] = isGroupCounter;

        if (isGroupCounter) {
            anyCounterSeenSoFar = true;
        }

        // Only a "live" group-start tag (one whose counter actually appeared in
        // this message) AND only once we've actually passed that counter in the
        // token stream counts as a real repeating-group instance boundary.
        bool isGroupStart = anyCounterSeenSoFar && liveGroupStartTags.count(tok.tag) > 0;
        entry["isGroupStart"] = isGroupStart;
        if (isGroupStart) {
            currentGroupIndex++;
        }
        bool withinGroupableSection = !isHeaderTag(tok.tag) && !isTrailerTag(tok.tag);
        entry["groupIndex"] = (withinGroupableSection && anyCounterSeenSoFar && currentGroupIndex >= 0) ? currentGroupIndex : -1;

        // Capture key fields for session/timeline use
        if (tok.tag == 35) msgType = rawVal;
        if (tok.tag == 49) senderCompID = rawVal;
        if (tok.tag == 56) targetCompID = rawVal;
        if (tok.tag == 52) sendingTime = rawVal;
        if (tok.tag == 11) clOrdID = rawVal;
        if (tok.tag == 41) origClOrdID = rawVal;

        entry["stepIndex"] = static_cast<int>(idx);

        crow::json::wvalue entryCopy1 = entry;
        crow::json::wvalue entryCopy2 = entry;

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
                std::string msgTypeNameLocal = dict.count(35) && dict.at(35).values.count(msgType)
                    ? dict.at(35).values.at(msgType) : msgType;
                errors.push_back(crow::json::wvalue(
                    "Missing required field for " + msgTypeNameLocal + ": " + name + " (tag " + std::to_string(reqTag) + ")"));
                isValid = false;
            }
        }
    }

    // ---------- Conditional validation rules ----------
    // These catch logical inconsistencies that simple "is the tag present" checks miss —
    // e.g. an OrdType of Stop without a StopPx is technically "all required base fields present"
    // but is still a broken/incomplete order from a real trading system's perspective.
    {
        auto hasTag = [&](int t) {
            for (const auto& tok : tokens) if (tok.tag == t) return true;
            return false;
        };
        auto valueOf = [&](int t) -> std::string {
            for (const auto& tok : tokens) if (tok.tag == t) return std::string(tok.value);
            return "";
        };

        std::string ordType = valueOf(40);
        std::string timeInForce = valueOf(59);

        // OrdType = Stop (3) or Stop Limit (4) requires StopPx (99)
        if ((ordType == "3" || ordType == "4") && !hasTag(99)) {
            errors.push_back(crow::json::wvalue(
                "OrdType is Stop/Stop Limit but StopPx (tag 99) is missing — the order has no trigger price."));
            isValid = false;
        }

        // OrdType = Limit (2) or Stop Limit (4) requires Price (44)
        if ((ordType == "2" || ordType == "4") && !hasTag(44)) {
            errors.push_back(crow::json::wvalue(
                "OrdType is Limit/Stop Limit but Price (tag 44) is missing — a limit order needs a limit price."));
            isValid = false;
        }

        // TimeInForce = Good Till Date (6) requires ExpireDate (432) or ExpireTime (126)
        if (timeInForce == "6" && !hasTag(432) && !hasTag(126)) {
            errors.push_back(crow::json::wvalue(
                "TimeInForce is Good Till Date but neither ExpireDate (432) nor ExpireTime (126) is present."));
            isValid = false;
        }

        // Execution Report (35=8) with ExecType=Rejected (150=8) should have a reason —
        // OrdRejReason (103) or free-text Text (58)
        if (msgType == "8") {
            std::string execType = valueOf(150);
            if (execType == "8" && !hasTag(103) && !hasTag(58)) {
                errors.push_back(crow::json::wvalue(
                    "ExecType is Rejected but no OrdRejReason (103) or Text (58) explains why."));
                isValid = false;
            }
        }

        // Order Cancel Reject (35=9) should always include CxlRejReason (102)
        if (msgType == "9" && !hasTag(102)) {
            errors.push_back(crow::json::wvalue(
                "Order Cancel Reject is missing CxlRejReason (tag 102) — the reason the cancel was rejected."));
            isValid = false;
        }

        // Repeating group sanity check: if a counter tag (e.g. NoMDEntries) is present,
        // verify the actual number of group-start entries matches the stated count.
        for (const auto& kv : groupStarts) {
            int counterTag = kv.first;
            int groupStartTag = kv.second;
            std::string countStr = valueOf(counterTag);
            if (countStr.empty()) continue;
            try {
                int statedCount = std::stoi(countStr);
                int actualCount = 0;
                for (const auto& tok : tokens) {
                    if (tok.tag == groupStartTag) actualCount++;
                }
                if (statedCount != actualCount) {
                    std::string counterName = dict.count(counterTag) ? dict.at(counterTag).name : std::to_string(counterTag);
                    errors.push_back(crow::json::wvalue(
                        counterName + " (tag " + std::to_string(counterTag) + ") states " + std::to_string(statedCount) +
                        " entries, but " + std::to_string(actualCount) + " were actually found in the message."));
                    isValid = false;
                }
            } catch (...) {
                // non-numeric counter value — ignore, not our concern here
            }
        }
    }

    // ---------- Assemble ----------
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

    // Extra top-level fields for session/timeline display
    result["senderCompID"] = senderCompID;
    result["targetCompID"] = targetCompID;
    result["sendingTime"] = sendingTime;
    result["clOrdID"] = clOrdID;
    result["origClOrdID"] = origClOrdID;
    result["rawMessage"] = body;

    return result;
}

// ---------- Auto-summary generator ----------
// Builds a short human-readable one-line summary for a parsed message,
// used in the multi-message Timeline view.
static std::string fieldRaw(const std::vector<FixToken>& tokens, int tag) {
    for (const auto& t : tokens) {
        if (t.tag == tag) return std::string(t.value);
    }
    return "";
}

static std::string formatQty(const std::string& q) {
    if (q.empty()) return q;
    try {
        long long val = std::stoll(q);
        std::string s = std::to_string(val);
        std::string out;
        int count = 0;
        for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
            out = s[i] + out;
            count++;
            if (count % 3 == 0 && i != 0) out = "," + out;
        }
        return out;
    } catch (...) {
        return q;
    }
}

static std::string buildSummary(const std::string& body, char delim) {
    std::vector<FixToken> tokens = tokenize(body, delim);
    std::string msgType = fieldRaw(tokens, 35);
    const auto& dict = getDictionary();

    auto sideWord = [&](const std::string& sideVal) -> std::string {
        if (sideVal == "1") return "Buy";
        if (sideVal == "2") return "Sell";
        if (sideVal == "5") return "Sell short";
        return sideVal.empty() ? "" : "Side(" + sideVal + ")";
    };

    if (msgType == "D") { // New Order Single
        std::string side = sideWord(fieldRaw(tokens, 54));
        std::string qty = formatQty(fieldRaw(tokens, 38));
        std::string sym = fieldRaw(tokens, 55);
        std::string px = fieldRaw(tokens, 44);
        std::string ordType = fieldRaw(tokens, 40);
        std::string s = side + " " + qty + " " + sym;
        if (!px.empty() && ordType != "1") s += " @ " + px;
        return s;
    }
    if (msgType == "8") { // Execution Report
        std::string execType = fieldRaw(tokens, 150);
        std::string ordStatus = fieldRaw(tokens, 39);
        std::string side = sideWord(fieldRaw(tokens, 54));
        std::string sym = fieldRaw(tokens, 55);
        std::string lastShares = fieldRaw(tokens, 32);
        std::string lastPx = fieldRaw(tokens, 31);
        std::string avgPx = fieldRaw(tokens, 6);
        std::string cumQty = fieldRaw(tokens, 14);

        if (execType == "0" || ordStatus == "0") {
            return side + " " + formatQty(fieldRaw(tokens, 38)) + " " + sym + " (Acknowledged)";
        }
        if (execType == "8" || ordStatus == "8") {
            std::string text = fieldRaw(tokens, 58);
            return "Rejected" + (text.empty() ? std::string("") : (": " + text));
        }
        if (execType == "4" || ordStatus == "4") {
            return "Canceled " + sym;
        }
        // Fill / Partial fill
        std::string qtyPart = !lastShares.empty() ? formatQty(lastShares) : formatQty(cumQty);
        std::string s = side + " " + qtyPart + " " + sym;
        if (!lastPx.empty()) s += " @ " + lastPx;
        if (!avgPx.empty()) s += " · avg px " + avgPx;
        return s;
    }
    if (msgType == "F" || msgType == "G") { // Cancel Request / Replace
        std::string side = sideWord(fieldRaw(tokens, 54));
        std::string sym = fieldRaw(tokens, 55);
        std::string qty = formatQty(fieldRaw(tokens, 38));
        std::string s = (msgType == "F" ? std::string("Cancel request: ") : std::string("Replace request: "));
        s += side;
        if (!side.empty() && !sym.empty()) s += " ";
        s += sym;
        if (!qty.empty() && qty != "0") s += " " + qty;
        return s;
    }
    if (msgType == "9") { // Order Cancel Reject
        std::string reason = fieldRaw(tokens, 102);
        std::string reasonText = (dict.count(102) && dict.at(102).values.count(reason))
            ? dict.at(102).values.at(reason) : reason;
        return "Cancel rejected" + (reasonText.empty() ? std::string("") : (": " + reasonText));
    }
    if (msgType == "3") { // Reject
        std::string text = fieldRaw(tokens, 58);
        std::string reasonCode = fieldRaw(tokens, 373);
        std::string reasonText = (dict.count(373) && dict.at(373).values.count(reasonCode))
            ? dict.at(373).values.at(reasonCode) : "";
        if (!text.empty()) return text;
        if (!reasonText.empty()) return reasonText;
        return "Unsupported message type";
    }
    if (msgType == "j") { // Business Message Reject
        std::string text = fieldRaw(tokens, 58);
        return text.empty() ? "Business message rejected" : text;
    }
    if (msgType == "A") return "Logon";
    if (msgType == "5") return "Logout";
    if (msgType == "0") return "Heartbeat";
    if (msgType == "1") return "Test Request";

    // Fallback: just show the message type name
    if (dict.count(35) && dict.at(35).values.count(msgType)) {
        return dict.at(35).values.at(msgType);
    }
    return "Unknown message";
}

// Checks whether an extracted segment looks like a real FIX message
// (must contain BeginString, BodyLength, and MsgType at minimum).
// Anything else (blank lines, log noise, partial fragments) is rejected.
static bool looksLikeFixMessage(const std::string& segment, char delim) {
    std::vector<FixToken> tokens = tokenize(segment, delim);
    bool has8 = false, has9 = false, has35 = false;
    for (const auto& t : tokens) {
        if (t.tag == 8) has8 = true;
        if (t.tag == 9) has9 = true;
        if (t.tag == 35) has35 = true;
    }
    return has8 && has9 && has35;
}

// Splits a raw multi-message log block into individual FIX messages.
// Handles garbage before/after each message (e.g. log timestamps, stray text,
// blank lines, or junk lines mixed in anywhere) by anchoring strictly on the
// "8=FIX...<delim>" start and the closing "10=XXX<delim>" checksum end of
// each message, and discarding any segment that doesn't contain the minimum
// required tags (8, 9, 35) to be considered a real FIX message.
static std::vector<std::string> splitLogIntoMessages(const std::string& log) {
    std::vector<std::string> messages;
    std::vector<size_t> starts;

    // Step 1: find every "8=FIX" occurrence — these are candidate message starts.
    size_t pos = 0;
    while (true) {
        size_t found = log.find("8=FIX", pos);
        if (found == std::string::npos) break;
        starts.push_back(found);
        pos = found + 5;
    }

    for (size_t i = 0; i < starts.size(); ++i) {
        size_t begin = starts[i];
        // Search window: from this start up to (but not including) the next
        // candidate start, or end of log if this is the last one.
        size_t windowEnd = (i + 1 < starts.size()) ? starts[i + 1] : log.size();
        std::string window = log.substr(begin, windowEnd - begin);

        // Step 2: detect the delimiter used in THIS window specifically
        // (different messages in the same log could theoretically use
        // different delimiters if pasted from different sources).
        char delim = detectDelimiter(window);

        // Step 3: find the closing "10=" tag within this window and trim
        // everything after its value + delimiter — this removes trailing
        // garbage (e.g. log metadata, extra text) that isn't part of the
        // actual FIX message.
        size_t tag10Pos = std::string::npos;
        for (size_t j = 0; j + 3 <= window.size(); ++j) {
            if (window[j] == '1' && window[j+1] == '0' && window[j+2] == '=') {
                if (j == 0 || window[j-1] == delim) {
                    tag10Pos = j;
                }
            }
        }

        std::string msg;
        if (tag10Pos != std::string::npos) {
            size_t valStart = tag10Pos + 3;
            size_t valEnd = window.find(delim, valStart);
            if (valEnd == std::string::npos) valEnd = window.size();
            else valEnd += 1; // include the trailing delimiter itself
            msg = window.substr(0, valEnd);
        } else {
            // No checksum found in this window — fall back to the whole
            // window (will likely fail validation downstream, but we don't
            // silently truncate real data we can't confirm the boundary of).
            msg = window;
        }

        // trim trailing whitespace/newlines just in case
        while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r' || msg.back() == ' ' || msg.back() == '\t')) {
            msg.pop_back();
        }

        // Step 4: reject anything that doesn't have the minimum required
        // tags to be considered a real FIX message (filters out blank
        // lines, stray log text, separators, partial fragments, etc).
        if (!msg.empty() && looksLikeFixMessage(msg, delim)) {
            messages.push_back(msg);
        }
    }

    return messages;
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

        // Reject payloads larger than 10 MB to prevent memory exhaustion.
        constexpr size_t MAX_BODY = 10 * 1024 * 1024;
        if (body.size() > MAX_BODY) {
            crow::json::wvalue err;
            err["status"] = "error";
            err["message"] = "Payload too large (max 10 MB). Use /api/parse-log for multi-message blobs.";
            response.body = err.dump();
            response.code = 413;
            return response;
        }

        // Auto-detect multi-message input: if the body contains more than one
        // "8=FIX" start marker, redirect internally to the log parser instead of
        // silently truncating everything after the first message.
        size_t fixCount = 0;
        size_t searchPos = 0;
        while ((searchPos = body.find("8=FIX", searchPos)) != std::string::npos) {
            ++fixCount;
            searchPos += 5;
            if (fixCount > 1) break;
        }

        if (fixCount > 1) {
            crow::json::wvalue warn;
            warn["status"] = "error";
            warn["message"] = "Multiple FIX messages detected. Please use /api/parse-log for multi-message input.";
            response.body = warn.dump();
            response.code = 400;
            return response;
        }

        crow::json::wvalue result = parseOneMessage(body);
        response.body = result.dump();
        response.code = 200;
        return response;
    });

    CROW_ROUTE(app, "/api/parse-log").methods("POST"_method)
    ([](const crow::request& req) {
        std::string logBody = req.body;

        crow::response response;
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Access-Control-Allow-Headers", "Content-Type");
        response.add_header("Content-Type", "application/json");

        if (logBody.empty()) {
            crow::json::wvalue err;
            err["status"] = "error";
            err["message"] = "Empty input";
            response.body = err.dump();
            response.code = 400;
            return response;
        }

        // Reject payloads larger than 10 MB to prevent memory exhaustion.
        constexpr size_t MAX_BODY = 10 * 1024 * 1024;
        if (logBody.size() > MAX_BODY) {
            crow::json::wvalue err;
            err["status"] = "error";
            err["message"] = "Payload too large (max 10 MB).";
            response.body = err.dump();
            response.code = 413;
            return response;
        }

        std::vector<std::string> messages = splitLogIntoMessages(logBody);

        if (messages.empty()) {
            crow::json::wvalue err;
            err["status"] = "error";
            err["message"] = "No valid FIX messages found (expected each message to start with 8=FIX)";
            response.body = err.dump();
            response.code = 400;
            return response;
        }

        std::vector<crow::json::wvalue> parsedMessages;

        for (size_t i = 0; i < messages.size(); ++i) {
            const std::string& msg = messages[i];
            crow::json::wvalue parsed = parseOneMessage(msg);

            char delim = detectDelimiter(msg);
            std::string summary = buildSummary(msg, delim);
            parsed["summary"] = summary;
            parsed["logIndex"] = static_cast<int>(i);

            parsedMessages.push_back(std::move(parsed));
        }

        crow::json::wvalue result;
        result["status"] = "ok";
        result["totalMessages"] = static_cast<int>(messages.size());
        result["messages"] = std::move(parsedMessages);

        response.body = result.dump();
        response.code = 200;
        return response;
    });

    CROW_ROUTE(app, "/api/parse-log").methods("OPTIONS"_method)
    ([](const crow::request&) {
        crow::response response(204);
        response.add_header("Access-Control-Allow-Origin", "*");
        response.add_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        response.add_header("Access-Control-Allow-Headers", "Content-Type");
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
