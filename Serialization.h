#pragma once
#include "ThirdParty/json/json.hpp"
#include "ThirdParty/tinyfiledialogs/tinyfiledialogs.h"
#include <fstream>
#include <iostream>
#include "TireModels/Dugoff.h"
#include "TireModels/Fiala.h"
#include "TireModels/MF.h"
#include "TireModels/Pacejka.h"
#include "TireModels/SimplePacejka.h"
#include "TireModels/Tire521.h"
#include "TireModels/UA.H"

using json = nlohmann::json;

struct SimplePacejkaContext
{
    SimplePacejkaParams params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 0.0 };
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    TMOutput current_out{};
};

struct MFContext
{
    MFParams params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 230000.0, 0.0, 16.67, 0.3 };
    MFFeatureLevel featureLevel = MFFeatureLevel::MF62;
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    float camber_deg_f = 0.0f;
    float pressure_f = 230000.0f;
    float psi_f = 0.0f;
    float vx_f = 16.67f;
    float r_f = 0.3f;
    bool TurnSlip = false;
    TMOutput current_out{};
};

struct ClassicPacejkaContext
{
    PacejkaParams params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 230000.0, 0.0, 16.67, 0.3 };
    PacejkaFeatureLevel featureLevel = PacejkaFeatureLevel::Pacejka94;
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    float camber_deg_f = 0.0f;
    float vx_f = 16.67f;
    TMOutput current_out{};
};

struct UAContext
{
    UAParams params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 230000.0, 0.0, 16.67, 0.3 };
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    float camber_deg_f = 0.0f;
    float vx_f = 16.67f;
    float r_f = 0.3f;
    TMOutput current_out{};
};

struct FialaContext
{
    FialaParams params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 230000.0, 0.0, 16.67, 0.3 };
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    float vx_f = 16.67f;
    float r_f = 0.3f;
    TMOutput current_out{};
};

struct DugoffContext
{
    DGParams params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 16.67, 0.3 };
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    TMOutput current_out{};
};

struct Tire521Context
{
    Tire521Params params{};
    TMState state = { 4000.0, 0.0, 0.0, 0.0, 0.0, 0.0, 16.67, 0.3 };
    float alpha_deg_f = 0.0f;
    float slip_ratio_f = 0.0f;
    float fz_f = 4000.0f;
    float vx_f = 16.67f;
    float r_f = 0.3f;
    TMOutput current_out{};
};

void to_json(json& j, const SimplePacejkaParams& p)
{
    j = json{ {"Bx", p.Bx}, {"Cx", p.Cx}, {"Dx", p.Dx}, {"Ex", p.Ex},
             {"By", p.By}, {"Cy", p.Cy}, {"Dy", p.Dy}, {"Ey", p.Ey} };
}

void from_json(const json& j, SimplePacejkaParams& p)
{
    if (j.contains("Bx")) j.at("Bx").get_to(p.Bx);
    if (j.contains("Cx")) j.at("Cx").get_to(p.Cx);
    if (j.contains("Dx")) j.at("Dx").get_to(p.Dx);
    if (j.contains("Ex")) j.at("Ex").get_to(p.Ex);
    if (j.contains("By")) j.at("By").get_to(p.By);
    if (j.contains("Cy")) j.at("Cy").get_to(p.Cy);
    if (j.contains("Dy")) j.at("Dy").get_to(p.Dy);
    if (j.contains("Ey")) j.at("Ey").get_to(p.Ey);
}

void to_json(json& j, const SimplePacejkaContext& c)
{
    j["params"] = c.params;
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
}

void from_json(const json& j, SimplePacejkaContext& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
}

void to_json(json& j, const DGParams& p)
{
    j = json{ {"CX", p.CX}, {"CY", p.CY}, {"MuF", p.MuF} };
}

void from_json(const json& j, DGParams& p)
{
    if (j.contains("CX")) j.at("CX").get_to(p.CX);
    if (j.contains("CY")) j.at("CY").get_to(p.CY);
    if (j.contains("MuF")) j.at("MuF").get_to(p.MuF);
}

void to_json(json& j, const DugoffContext& c)
{
    j["params"] = c.params;
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
}

void from_json(const json& j, DugoffContext& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
}

void to_json(json& j, const FialaParams& p)
{
    j = json{ {"CSLIP", p.CSLIP}, {"CALPHA", p.CALPHA}, {"UMin", p.UMin}, {"UMax", p.UMax}, {"Width", p.Width}, {"RollingResistance", p.RollingResistance} };
}

void from_json(const json& j, FialaParams& p)
{
    if (j.contains("CSLIP")) j.at("CSLIP").get_to(p.CSLIP);
    if (j.contains("CALPHA")) j.at("CALPHA").get_to(p.CALPHA);
    if (j.contains("UMin")) j.at("UMin").get_to(p.UMin);
    if (j.contains("UMax")) j.at("UMax").get_to(p.UMax);
    if (j.contains("Width")) j.at("Width").get_to(p.Width);
    if (j.contains("RollingResistance")) j.at("RollingResistance").get_to(p.RollingResistance);
}

void to_json(json& j, const FialaContext& c)
{
    j["params"] = c.params;
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
    j["vx_f"] = c.vx_f;
    j["r_f"] = c.r_f;
}

void from_json(const json& j, FialaContext& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
    if (j.contains("vx_f")) j.at("vx_f").get_to(c.vx_f);
    if (j.contains("r_f")) j.at("r_f").get_to(c.r_f);
}

void to_json(json& j, const UAParams& p)
{
    j = json{ {"CSLIP", p.CSLIP}, {"CALPHA", p.CALPHA}, {"CGAMMA", p.CGAMMA},
             {"UMin", p.UMin}, {"UMax", p.UMax}, {"RollingResistance", p.RollingResistance}, {"VerticalStiffness", p.VerticalStiffness} };
}

void from_json(const json& j, UAParams& p)
{
    if (j.contains("CSLIP")) j.at("CSLIP").get_to(p.CSLIP);
    if (j.contains("CALPHA")) j.at("CALPHA").get_to(p.CALPHA);
    if (j.contains("CGAMMA")) j.at("CGAMMA").get_to(p.CGAMMA);
    if (j.contains("UMin")) j.at("UMin").get_to(p.UMin);
    if (j.contains("UMax")) j.at("UMax").get_to(p.UMax);
    if (j.contains("RollingResistance")) j.at("RollingResistance").get_to(p.RollingResistance);
    if (j.contains("VerticalStiffness")) j.at("VerticalStiffness").get_to(p.VerticalStiffness);
}

void to_json(json& j, const UAContext& c)
{
    j["params"] = c.params;
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
    j["camber_deg_f"] = c.camber_deg_f;
    j["vx_f"] = c.vx_f;
    j["r_f"] = c.r_f;
}

void from_json(const json& j, UAContext& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
    if (j.contains("camber_deg_f")) j.at("camber_deg_f").get_to(c.camber_deg_f);
    if (j.contains("vx_f")) j.at("vx_f").get_to(c.vx_f);
    if (j.contains("r_f")) j.at("r_f").get_to(c.r_f);
}

void to_json(json& j, const Tire521Params& p)
{
    j = json{ {"VerticalStiffness", p.VerticalStiffness}, {"VerticalDamping", p.VerticalDamping}, {"VerticalStiffnessExponent", p.VerticalStiffnessExponent},
             {"LateralStiffness", p.LateralStiffness}, {"CorneringStiffnessCoefficient", p.CorneringStiffnessCoefficient},
             {"MuStatic", p.MuStatic}, {"MuDynamic", p.MuDynamic}, {"MuStaticVelocity", p.MuStaticVelocity}, {"MuDynamicVelocity", p.MuDynamicVelocity},
             {"RollingResistanceCoefficient", p.RollingResistanceCoefficient}, {"UnloadedRadius", p.UnloadedRadius} };
}

void from_json(const json& j, Tire521Params& p)
{
    if (j.contains("VerticalStiffness")) j.at("VerticalStiffness").get_to(p.VerticalStiffness);
    if (j.contains("VerticalDamping")) j.at("VerticalDamping").get_to(p.VerticalDamping);
    if (j.contains("VerticalStiffnessExponent")) j.at("VerticalStiffnessExponent").get_to(p.VerticalStiffnessExponent);
    if (j.contains("LateralStiffness")) j.at("LateralStiffness").get_to(p.LateralStiffness);
    if (j.contains("CorneringStiffnessCoefficient")) j.at("CorneringStiffnessCoefficient").get_to(p.CorneringStiffnessCoefficient);
    if (j.contains("MuStatic")) j.at("MuStatic").get_to(p.MuStatic);
    if (j.contains("MuDynamic")) j.at("MuDynamic").get_to(p.MuDynamic);
    if (j.contains("MuStaticVelocity")) j.at("MuStaticVelocity").get_to(p.MuStaticVelocity);
    if (j.contains("MuDynamicVelocity")) j.at("MuDynamicVelocity").get_to(p.MuDynamicVelocity);
    if (j.contains("RollingResistanceCoefficient")) j.at("RollingResistanceCoefficient").get_to(p.RollingResistanceCoefficient);
    if (j.contains("UnloadedRadius")) j.at("UnloadedRadius").get_to(p.UnloadedRadius);
}

void to_json(json& j, const Tire521Context& c)
{
    j["params"] = c.params;
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
    j["vx_f"] = c.vx_f;
    j["r_f"] = c.r_f;
}

void from_json(const json& j, Tire521Context& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
    if (j.contains("vx_f")) j.at("vx_f").get_to(c.vx_f);
    if (j.contains("r_f")) j.at("r_f").get_to(c.r_f);
}

void to_json(json& j, const PacejkaParams& p)
{
    j = json{
        {"DLON", p.DLON}, {"DLAT", p.DLAT}, {"BCDLON", p.BCDLON}, {"BCDLAT", p.BCDLAT},
        {"B0", p.B0}, {"B1", p.B1}, {"B2", p.B2}, {"B3", p.B3}, {"B4", p.B4}, {"B5", p.B5}, {"B6", p.B6}, {"B7", p.B7}, {"B8", p.B8}, {"B9", p.B9}, {"B10", p.B10}, {"B11", p.B11}, {"B12", p.B12}, {"B13", p.B13},
        {"A0", p.A0}, {"A1", p.A1}, {"A2", p.A2}, {"A3", p.A3}, {"A4", p.A4}, {"A5", p.A5}, {"A6", p.A6}, {"A7", p.A7}, {"A8", p.A8}, {"A9", p.A9}, {"A10", p.A10}, {"A11", p.A11}, {"A12", p.A12}, {"A13", p.A13}, {"A14", p.A14}, {"A15", p.A15}, {"A16", p.A16}, {"A17", p.A17},
        {"C0", p.C0}, {"C1", p.C1}, {"C2", p.C2}, {"C3", p.C3}, {"C4", p.C4}, {"C5", p.C5}, {"C6", p.C6}, {"C7", p.C7}, {"C8", p.C8}, {"C9", p.C9}, {"C10", p.C10}, {"C11", p.C11}, {"C12", p.C12}, {"C13", p.C13}, {"C14", p.C14}, {"C15", p.C15}, {"C16", p.C16}, {"C17", p.C17}, {"C18", p.C18}, {"C19", p.C19}, {"C20", p.C20},
        {"Fz0", p.Fz0}, {"R0", p.R0}, {"Vref", p.Vref}, {"LateralStiffness", p.LateralStiffness}, {"VerticalStiffness", p.VerticalStiffness}, {"RollingResistance", p.RollingResistance}
    };
}

void from_json(const json& j, PacejkaParams& p)
{
    if (j.contains("DLON")) j.at("DLON").get_to(p.DLON);
    if (j.contains("DLAT")) j.at("DLAT").get_to(p.DLAT);
    if (j.contains("BCDLON")) j.at("BCDLON").get_to(p.BCDLON);
    if (j.contains("BCDLAT")) j.at("BCDLAT").get_to(p.BCDLAT);
    if (j.contains("B0")) j.at("B0").get_to(p.B0);
    if (j.contains("B1")) j.at("B1").get_to(p.B1);
    if (j.contains("B2")) j.at("B2").get_to(p.B2);
    if (j.contains("B3")) j.at("B3").get_to(p.B3);
    if (j.contains("B4")) j.at("B4").get_to(p.B4);
    if (j.contains("B5")) j.at("B5").get_to(p.B5);
    if (j.contains("B6")) j.at("B6").get_to(p.B6);
    if (j.contains("B7")) j.at("B7").get_to(p.B7);
    if (j.contains("B8")) j.at("B8").get_to(p.B8);
    if (j.contains("B9")) j.at("B9").get_to(p.B9);
    if (j.contains("B10")) j.at("B10").get_to(p.B10);
    if (j.contains("B11")) j.at("B11").get_to(p.B11);
    if (j.contains("B12")) j.at("B12").get_to(p.B12);
    if (j.contains("B13")) j.at("B13").get_to(p.B13);
    if (j.contains("A0")) j.at("A0").get_to(p.A0);
    if (j.contains("A1")) j.at("A1").get_to(p.A1);
    if (j.contains("A2")) j.at("A2").get_to(p.A2);
    if (j.contains("A3")) j.at("A3").get_to(p.A3);
    if (j.contains("A4")) j.at("A4").get_to(p.A4);
    if (j.contains("A5")) j.at("A5").get_to(p.A5);
    if (j.contains("A6")) j.at("A6").get_to(p.A6);
    if (j.contains("A7")) j.at("A7").get_to(p.A7);
    if (j.contains("A8")) j.at("A8").get_to(p.A8);
    if (j.contains("A9")) j.at("A9").get_to(p.A9);
    if (j.contains("A10")) j.at("A10").get_to(p.A10);
    if (j.contains("A11")) j.at("A11").get_to(p.A11);
    if (j.contains("A12")) j.at("A12").get_to(p.A12);
    if (j.contains("A13")) j.at("A13").get_to(p.A13);
    if (j.contains("A14")) j.at("A14").get_to(p.A14);
    if (j.contains("A15")) j.at("A15").get_to(p.A15);
    if (j.contains("A16")) j.at("A16").get_to(p.A16);
    if (j.contains("A17")) j.at("A17").get_to(p.A17);
    if (j.contains("C0")) j.at("C0").get_to(p.C0);
    if (j.contains("C1")) j.at("C1").get_to(p.C1);
    if (j.contains("C2")) j.at("C2").get_to(p.C2);
    if (j.contains("C3")) j.at("C3").get_to(p.C3);
    if (j.contains("C4")) j.at("C4").get_to(p.C4);
    if (j.contains("C5")) j.at("C5").get_to(p.C5);
    if (j.contains("C6")) j.at("C6").get_to(p.C6);
    if (j.contains("C7")) j.at("C7").get_to(p.C7);
    if (j.contains("C8")) j.at("C8").get_to(p.C8);
    if (j.contains("C9")) j.at("C9").get_to(p.C9);
    if (j.contains("C10")) j.at("C10").get_to(p.C10);
    if (j.contains("C11")) j.at("C11").get_to(p.C11);
    if (j.contains("C12")) j.at("C12").get_to(p.C12);
    if (j.contains("C13")) j.at("C13").get_to(p.C13);
    if (j.contains("C14")) j.at("C14").get_to(p.C14);
    if (j.contains("C15")) j.at("C15").get_to(p.C15);
    if (j.contains("C16")) j.at("C16").get_to(p.C16);
    if (j.contains("C17")) j.at("C17").get_to(p.C17);
    if (j.contains("C18")) j.at("C18").get_to(p.C18);
    if (j.contains("C19")) j.at("C19").get_to(p.C19);
    if (j.contains("C20")) j.at("C20").get_to(p.C20);
    if (j.contains("Fz0")) j.at("Fz0").get_to(p.Fz0);
    if (j.contains("R0")) j.at("R0").get_to(p.R0);
    if (j.contains("Vref")) j.at("Vref").get_to(p.Vref);
    if (j.contains("LateralStiffness")) j.at("LateralStiffness").get_to(p.LateralStiffness);
    if (j.contains("VerticalStiffness")) j.at("VerticalStiffness").get_to(p.VerticalStiffness);
    if (j.contains("RollingResistance")) j.at("RollingResistance").get_to(p.RollingResistance);
}

void to_json(json& j, const ClassicPacejkaContext& c)
{
    j["params"] = c.params;
    j["featureLevel"] = static_cast<int>(c.featureLevel);
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
    j["camber_deg_f"] = c.camber_deg_f;
    j["vx_f"] = c.vx_f;
}

void from_json(const json& j, ClassicPacejkaContext& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("featureLevel"))
    {
        int fl;
        j.at("featureLevel").get_to(fl);
        c.featureLevel = static_cast<PacejkaFeatureLevel>(fl);
    }
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
    if (j.contains("camber_deg_f")) j.at("camber_deg_f").get_to(c.camber_deg_f);
    if (j.contains("vx_f")) j.at("vx_f").get_to(c.vx_f);
}

void to_json(json& j, const MFParams& p)
{
    j = json{
        {"LCX", p.LCX}, {"LMUX", p.LMUX}, {"LEX", p.LEX}, {"LKX", p.LKX}, {"LHX", p.LHX}, {"LVX", p.LVX}, {"LXAL", p.LXAL},
        {"LCY", p.LCY}, {"LMUY", p.LMUY}, {"LEY", p.LEY}, {"LKY", p.LKY}, {"LKYC", p.LKYC}, {"LKZC", p.LKZC}, {"LHY", p.LHY}, {"LVY", p.LVY}, {"LYKA", p.LYKA}, {"LVYKA", p.LVYKA}, {"LKC", p.LKC}, {"LCC", p.LCC}, {"LEC", p.LEC},
        {"LMX", p.LMX}, {"LVMX", p.LVMX}, {"LMY", p.LMY}, {"LTR", p.LTR}, {"LRES", p.LRES}, {"LS", p.LS}, {"LMP", p.LMP}, {"LGYR", p.LGYR},
        {"PCX1", p.PCX1}, {"PDX1", p.PDX1}, {"PDX2", p.PDX2}, {"PDX3", p.PDX3}, {"PEX1", p.PEX1}, {"PEX2", p.PEX2}, {"PEX3", p.PEX3}, {"PEX4", p.PEX4}, {"PKX1", p.PKX1}, {"PKX2", p.PKX2}, {"PKX3", p.PKX3}, {"PHX1", p.PHX1}, {"PHX2", p.PHX2}, {"PVX1", p.PVX1}, {"PVX2", p.PVX2},
        {"RBX1", p.RBX1}, {"RBX2", p.RBX2}, {"RBX3", p.RBX3}, {"RCX1", p.RCX1}, {"REX1", p.REX1}, {"REX2", p.REX2}, {"RHX1", p.RHX1},
        {"PPX1", p.PPX1}, {"PPX2", p.PPX2}, {"PPX3", p.PPX3}, {"PPX4", p.PPX4}, {"PDXP1", p.PDXP1}, {"PDXP2", p.PDXP2}, {"PDXP3", p.PDXP3},
        {"PCY1", p.PCY1}, {"PCY2", p.PCY2}, {"PDY1", p.PDY1}, {"PDY2", p.PDY2}, {"PDY3", p.PDY3}, {"PEY1", p.PEY1}, {"PEY2", p.PEY2}, {"PEY3", p.PEY3}, {"PEY4", p.PEY4}, {"PEY5", p.PEY5}, {"PKY1", p.PKY1}, {"PKY2", p.PKY2}, {"PKY3", p.PKY3}, {"PKY4", p.PKY4}, {"PKY5", p.PKY5}, {"PKY6", p.PKY6}, {"PKY7", p.PKY7}, {"PHY1", p.PHY1}, {"PHY2", p.PHY2}, {"PHY3", p.PHY3}, {"PVY1", p.PVY1}, {"PVY2", p.PVY2}, {"PVY3", p.PVY3}, {"PVY4", p.PVY4},
        {"RBY1", p.RBY1}, {"RBY2", p.RBY2}, {"RBY3", p.RBY3}, {"RBY4", p.RBY4}, {"RCY1", p.RCY1}, {"REY1", p.REY1}, {"REY2", p.REY2}, {"RHY1", p.RHY1}, {"RHY2", p.RHY2}, {"RVY1", p.RVY1}, {"RVY2", p.RVY2}, {"RVY3", p.RVY3}, {"RVY4", p.RVY4}, {"RVY5", p.RVY5}, {"RVY6", p.RVY6},
        {"PPY1", p.PPY1}, {"PPY2", p.PPY2}, {"PPY3", p.PPY3}, {"PPY4", p.PPY4}, {"PPY5", p.PPY5}, {"PKYP1", p.PKYP1}, {"PDYP1", p.PDYP1}, {"PDYP2", p.PDYP2}, {"PDYP3", p.PDYP3}, {"PDYP4", p.PDYP4}, {"PHYP1", p.PHYP1}, {"PHYP2", p.PHYP2}, {"PHYP3", p.PHYP3}, {"PHYP4", p.PHYP4},
        {"QSX1", p.QSX1}, {"QSX2", p.QSX2}, {"QSX3", p.QSX3}, {"QSX4", p.QSX4}, {"QSX5", p.QSX5}, {"QSX6", p.QSX6}, {"QSX7", p.QSX7}, {"QSX8", p.QSX8}, {"QSX9", p.QSX9}, {"QSX10", p.QSX10}, {"QSX11", p.QSX11}, {"QSX12", p.QSX12}, {"QSX13", p.QSX13}, {"QSX14", p.QSX14}, {"PPMX1", p.PPMX1},
        {"QSY1", p.QSY1}, {"QSY2", p.QSY2}, {"QSY3", p.QSY3}, {"QSY4", p.QSY4}, {"QSY5", p.QSY5}, {"QSY6", p.QSY6}, {"QSY7", p.QSY7}, {"QSY8", p.QSY8},
        {"QBZ1", p.QBZ1}, {"QBZ2", p.QBZ2}, {"QBZ3", p.QBZ3}, {"QBZ4", p.QBZ4}, {"QBZ5", p.QBZ5}, {"QBZ9", p.QBZ9}, {"QBZ10", p.QBZ10}, {"QCZ1", p.QCZ1}, {"QDZ1", p.QDZ1}, {"QDZ2", p.QDZ2}, {"QDZ3", p.QDZ3}, {"QDZ4", p.QDZ4}, {"QDZ6", p.QDZ6}, {"QDZ7", p.QDZ7}, {"QDZ8", p.QDZ8}, {"QDZ9", p.QDZ9}, {"QDZ10", p.QDZ10}, {"QDZ11", p.QDZ11}, {"QEZ1", p.QEZ1}, {"QEZ2", p.QEZ2}, {"QEZ3", p.QEZ3}, {"QEZ4", p.QEZ4}, {"QEZ5", p.QEZ5}, {"QHZ1", p.QHZ1}, {"QHZ2", p.QHZ2}, {"QHZ3", p.QHZ3}, {"QHZ4", p.QHZ4}, {"SSZ1", p.SSZ1}, {"SSZ2", p.SSZ2}, {"SSZ3", p.SSZ3}, {"SSZ4", p.SSZ4}, {"PPZ1", p.PPZ1}, {"PPZ2", p.PPZ2}, {"QDTP1", p.QDTP1}, {"QCRP1", p.QCRP1}, {"QCRP2", p.QCRP2}, {"QBRP1", p.QBRP1}, {"QDRP1", p.QDRP1},
        {"PECP1", p.PECP1}, {"PECP2", p.PECP2},
        {"FZ0", p.FZ0}, {"P0", p.P0}, {"VREF", p.VREF}, {"R0", p.R0}, {"PRESMIN", p.PRESMIN}, {"PRESMAX", p.PRESMAX}, {"FZMIN", p.FZMIN}, {"FZMAX", p.FZMAX}, {"KPUMIN", p.KPUMIN}, {"KPUMAX", p.KPUMAX}, {"ALPMIN", p.ALPMIN}, {"ALPMAX", p.ALPMAX}, {"CAMMIN", p.CAMMIN}, {"CAMMAX", p.CAMMAX}, {"LFZO", p.LFZO}
    };
}

void from_json(const json& j, MFParams& p)
{
    if (j.contains("LCX")) j.at("LCX").get_to(p.LCX);
    if (j.contains("LMUX")) j.at("LMUX").get_to(p.LMUX);
    if (j.contains("LEX")) j.at("LEX").get_to(p.LEX);
    if (j.contains("LKX")) j.at("LKX").get_to(p.LKX);
    if (j.contains("LHX")) j.at("LHX").get_to(p.LHX);
    if (j.contains("LVX")) j.at("LVX").get_to(p.LVX);
    if (j.contains("LXAL")) j.at("LXAL").get_to(p.LXAL);
    if (j.contains("LCY")) j.at("LCY").get_to(p.LCY);
    if (j.contains("LMUY")) j.at("LMUY").get_to(p.LMUY);
    if (j.contains("LEY")) j.at("LEY").get_to(p.LEY);
    if (j.contains("LKY")) j.at("LKY").get_to(p.LKY);
    if (j.contains("LKYC")) j.at("LKYC").get_to(p.LKYC);
    if (j.contains("LKZC")) j.at("LKZC").get_to(p.LKZC);
    if (j.contains("LHY")) j.at("LHY").get_to(p.LHY);
    if (j.contains("LVY")) j.at("LVY").get_to(p.LVY);
    if (j.contains("LYKA")) j.at("LYKA").get_to(p.LYKA);
    if (j.contains("LVYKA")) j.at("LVYKA").get_to(p.LVYKA);
    if (j.contains("LKC")) j.at("LKC").get_to(p.LKC);
    if (j.contains("LCC")) j.at("LCC").get_to(p.LCC);
    if (j.contains("LEC")) j.at("LEC").get_to(p.LEC);
    if (j.contains("LMX")) j.at("LMX").get_to(p.LMX);
    if (j.contains("LVMX")) j.at("LVMX").get_to(p.LVMX);
    if (j.contains("LMY")) j.at("LMY").get_to(p.LMY);
    if (j.contains("LTR")) j.at("LTR").get_to(p.LTR);
    if (j.contains("LRES")) j.at("LRES").get_to(p.LRES);
    if (j.contains("LS")) j.at("LS").get_to(p.LS);
    if (j.contains("LMP")) j.at("LMP").get_to(p.LMP);
    if (j.contains("LGYR")) j.at("LGYR").get_to(p.LGYR);
    if (j.contains("PCX1")) j.at("PCX1").get_to(p.PCX1);
    if (j.contains("PDX1")) j.at("PDX1").get_to(p.PDX1);
    if (j.contains("PDX2")) j.at("PDX2").get_to(p.PDX2);
    if (j.contains("PDX3")) j.at("PDX3").get_to(p.PDX3);
    if (j.contains("PEX1")) j.at("PEX1").get_to(p.PEX1);
    if (j.contains("PEX2")) j.at("PEX2").get_to(p.PEX2);
    if (j.contains("PEX3")) j.at("PEX3").get_to(p.PEX3);
    if (j.contains("PEX4")) j.at("PEX4").get_to(p.PEX4);
    if (j.contains("PKX1")) j.at("PKX1").get_to(p.PKX1);
    if (j.contains("PKX2")) j.at("PKX2").get_to(p.PKX2);
    if (j.contains("PKX3")) j.at("PKX3").get_to(p.PKX3);
    if (j.contains("PHX1")) j.at("PHX1").get_to(p.PHX1);
    if (j.contains("PHX2")) j.at("PHX2").get_to(p.PHX2);
    if (j.contains("PVX1")) j.at("PVX1").get_to(p.PVX1);
    if (j.contains("PVX2")) j.at("PVX2").get_to(p.PVX2);
    if (j.contains("RBX1")) j.at("RBX1").get_to(p.RBX1);
    if (j.contains("RBX2")) j.at("RBX2").get_to(p.RBX2);
    if (j.contains("RBX3")) j.at("RBX3").get_to(p.RBX3);
    if (j.contains("RCX1")) j.at("RCX1").get_to(p.RCX1);
    if (j.contains("REX1")) j.at("REX1").get_to(p.REX1);
    if (j.contains("REX2")) j.at("REX2").get_to(p.REX2);
    if (j.contains("RHX1")) j.at("RHX1").get_to(p.RHX1);
    if (j.contains("PPX1")) j.at("PPX1").get_to(p.PPX1);
    if (j.contains("PPX2")) j.at("PPX2").get_to(p.PPX2);
    if (j.contains("PPX3")) j.at("PPX3").get_to(p.PPX3);
    if (j.contains("PPX4")) j.at("PPX4").get_to(p.PPX4);
    if (j.contains("PDXP1")) j.at("PDXP1").get_to(p.PDXP1);
    if (j.contains("PDXP2")) j.at("PDXP2").get_to(p.PDXP2);
    if (j.contains("PDXP3")) j.at("PDXP3").get_to(p.PDXP3);
    if (j.contains("PCY1")) j.at("PCY1").get_to(p.PCY1);
    if (j.contains("PCY2")) j.at("PCY2").get_to(p.PCY2);
    if (j.contains("PDY1")) j.at("PDY1").get_to(p.PDY1);
    if (j.contains("PDY2")) j.at("PDY2").get_to(p.PDY2);
    if (j.contains("PDY3")) j.at("PDY3").get_to(p.PDY3);
    if (j.contains("PEY1")) j.at("PEY1").get_to(p.PEY1);
    if (j.contains("PEY2")) j.at("PEY2").get_to(p.PEY2);
    if (j.contains("PEY3")) j.at("PEY3").get_to(p.PEY3);
    if (j.contains("PEY4")) j.at("PEY4").get_to(p.PEY4);
    if (j.contains("PEY5")) j.at("PEY5").get_to(p.PEY5);
    if (j.contains("PKY1")) j.at("PKY1").get_to(p.PKY1);
    if (j.contains("PKY2")) j.at("PKY2").get_to(p.PKY2);
    if (j.contains("PKY3")) j.at("PKY3").get_to(p.PKY3);
    if (j.contains("PKY4")) j.at("PKY4").get_to(p.PKY4);
    if (j.contains("PKY5")) j.at("PKY5").get_to(p.PKY5);
    if (j.contains("PKY6")) j.at("PKY6").get_to(p.PKY6);
    if (j.contains("PKY7")) j.at("PKY7").get_to(p.PKY7);
    if (j.contains("PHY1")) j.at("PHY1").get_to(p.PHY1);
    if (j.contains("PHY2")) j.at("PHY2").get_to(p.PHY2);
    if (j.contains("PHY3")) j.at("PHY3").get_to(p.PHY3);
    if (j.contains("PVY1")) j.at("PVY1").get_to(p.PVY1);
    if (j.contains("PVY2")) j.at("PVY2").get_to(p.PVY2);
    if (j.contains("PVY3")) j.at("PVY3").get_to(p.PVY3);
    if (j.contains("PVY4")) j.at("PVY4").get_to(p.PVY4);
    if (j.contains("RBY1")) j.at("RBY1").get_to(p.RBY1);
    if (j.contains("RBY2")) j.at("RBY2").get_to(p.RBY2);
    if (j.contains("RBY3")) j.at("RBY3").get_to(p.RBY3);
    if (j.contains("RBY4")) j.at("RBY4").get_to(p.RBY4);
    if (j.contains("RCY1")) j.at("RCY1").get_to(p.RCY1);
    if (j.contains("REY1")) j.at("REY1").get_to(p.REY1);
    if (j.contains("REY2")) j.at("REY2").get_to(p.REY2);
    if (j.contains("RHY1")) j.at("RHY1").get_to(p.RHY1);
    if (j.contains("RHY2")) j.at("RHY2").get_to(p.RHY2);
    if (j.contains("RVY1")) j.at("RVY1").get_to(p.RVY1);
    if (j.contains("RVY2")) j.at("RVY2").get_to(p.RVY2);
    if (j.contains("RVY3")) j.at("RVY3").get_to(p.RVY3);
    if (j.contains("RVY4")) j.at("RVY4").get_to(p.RVY4);
    if (j.contains("RVY5")) j.at("RVY5").get_to(p.RVY5);
    if (j.contains("RVY6")) j.at("RVY6").get_to(p.RVY6);
    if (j.contains("PPY1")) j.at("PPY1").get_to(p.PPY1);
    if (j.contains("PPY2")) j.at("PPY2").get_to(p.PPY2);
    if (j.contains("PPY3")) j.at("PPY3").get_to(p.PPY3);
    if (j.contains("PPY4")) j.at("PPY4").get_to(p.PPY4);
    if (j.contains("PPY5")) j.at("PPY5").get_to(p.PPY5);
    if (j.contains("PKYP1")) j.at("PKYP1").get_to(p.PKYP1);
    if (j.contains("PDYP1")) j.at("PDYP1").get_to(p.PDYP1);
    if (j.contains("PDYP2")) j.at("PDYP2").get_to(p.PDYP2);
    if (j.contains("PDYP3")) j.at("PDYP3").get_to(p.PDYP3);
    if (j.contains("PDYP4")) j.at("PDYP4").get_to(p.PDYP4);
    if (j.contains("PHYP1")) j.at("PHYP1").get_to(p.PHYP1);
    if (j.contains("PHYP2")) j.at("PHYP2").get_to(p.PHYP2);
    if (j.contains("PHYP3")) j.at("PHYP3").get_to(p.PHYP3);
    if (j.contains("PHYP4")) j.at("PHYP4").get_to(p.PHYP4);
    if (j.contains("QSX1")) j.at("QSX1").get_to(p.QSX1);
    if (j.contains("QSX2")) j.at("QSX2").get_to(p.QSX2);
    if (j.contains("QSX3")) j.at("QSX3").get_to(p.QSX3);
    if (j.contains("QSX4")) j.at("QSX4").get_to(p.QSX4);
    if (j.contains("QSX5")) j.at("QSX5").get_to(p.QSX5);
    if (j.contains("QSX6")) j.at("QSX6").get_to(p.QSX6);
    if (j.contains("QSX7")) j.at("QSX7").get_to(p.QSX7);
    if (j.contains("QSX8")) j.at("QSX8").get_to(p.QSX8);
    if (j.contains("QSX9")) j.at("QSX9").get_to(p.QSX9);
    if (j.contains("QSX10")) j.at("QSX10").get_to(p.QSX10);
    if (j.contains("QSX11")) j.at("QSX11").get_to(p.QSX11);
    if (j.contains("QSX12")) j.at("QSX12").get_to(p.QSX12);
    if (j.contains("QSX13")) j.at("QSX13").get_to(p.QSX13);
    if (j.contains("QSX14")) j.at("QSX14").get_to(p.QSX14);
    if (j.contains("PPMX1")) j.at("PPMX1").get_to(p.PPMX1);
    if (j.contains("QSY1")) j.at("QSY1").get_to(p.QSY1);
    if (j.contains("QSY2")) j.at("QSY2").get_to(p.QSY2);
    if (j.contains("QSY3")) j.at("QSY3").get_to(p.QSY3);
    if (j.contains("QSY4")) j.at("QSY4").get_to(p.QSY4);
    if (j.contains("QSY5")) j.at("QSY5").get_to(p.QSY5);
    if (j.contains("QSY6")) j.at("QSY6").get_to(p.QSY6);
    if (j.contains("QSY7")) j.at("QSY7").get_to(p.QSY7);
    if (j.contains("QSY8")) j.at("QSY8").get_to(p.QSY8);
    if (j.contains("QBZ1")) j.at("QBZ1").get_to(p.QBZ1);
    if (j.contains("QBZ2")) j.at("QBZ2").get_to(p.QBZ2);
    if (j.contains("QBZ3")) j.at("QBZ3").get_to(p.QBZ3);
    if (j.contains("QBZ4")) j.at("QBZ4").get_to(p.QBZ4);
    if (j.contains("QBZ5")) j.at("QBZ5").get_to(p.QBZ5);
    if (j.contains("QBZ9")) j.at("QBZ9").get_to(p.QBZ9);
    if (j.contains("QBZ10")) j.at("QBZ10").get_to(p.QBZ10);
    if (j.contains("QCZ1")) j.at("QCZ1").get_to(p.QCZ1);
    if (j.contains("QDZ1")) j.at("QDZ1").get_to(p.QDZ1);
    if (j.contains("QDZ2")) j.at("QDZ2").get_to(p.QDZ2);
    if (j.contains("QDZ3")) j.at("QDZ3").get_to(p.QDZ3);
    if (j.contains("QDZ4")) j.at("QDZ4").get_to(p.QDZ4);
    if (j.contains("QDZ6")) j.at("QDZ6").get_to(p.QDZ6);
    if (j.contains("QDZ7")) j.at("QDZ7").get_to(p.QDZ7);
    if (j.contains("QDZ8")) j.at("QDZ8").get_to(p.QDZ8);
    if (j.contains("QDZ9")) j.at("QDZ9").get_to(p.QDZ9);
    if (j.contains("QDZ10")) j.at("QDZ10").get_to(p.QDZ10);
    if (j.contains("QDZ11")) j.at("QDZ11").get_to(p.QDZ11);
    if (j.contains("QEZ1")) j.at("QEZ1").get_to(p.QEZ1);
    if (j.contains("QEZ2")) j.at("QEZ2").get_to(p.QEZ2);
    if (j.contains("QEZ3")) j.at("QEZ3").get_to(p.QEZ3);
    if (j.contains("QEZ4")) j.at("QEZ4").get_to(p.QEZ4);
    if (j.contains("QEZ5")) j.at("QEZ5").get_to(p.QEZ5);
    if (j.contains("QHZ1")) j.at("QHZ1").get_to(p.QHZ1);
    if (j.contains("QHZ2")) j.at("QHZ2").get_to(p.QHZ2);
    if (j.contains("QHZ3")) j.at("QHZ3").get_to(p.QHZ3);
    if (j.contains("QHZ4")) j.at("QHZ4").get_to(p.QHZ4);
    if (j.contains("SSZ1")) j.at("SSZ1").get_to(p.SSZ1);
    if (j.contains("SSZ2")) j.at("SSZ2").get_to(p.SSZ2);
    if (j.contains("SSZ3")) j.at("SSZ3").get_to(p.SSZ3);
    if (j.contains("SSZ4")) j.at("SSZ4").get_to(p.SSZ4);
    if (j.contains("PPZ1")) j.at("PPZ1").get_to(p.PPZ1);
    if (j.contains("PPZ2")) j.at("PPZ2").get_to(p.PPZ2);
    if (j.contains("QDTP1")) j.at("QDTP1").get_to(p.QDTP1);
    if (j.contains("QCRP1")) j.at("QCRP1").get_to(p.QCRP1);
    if (j.contains("QCRP2")) j.at("QCRP2").get_to(p.QCRP2);
    if (j.contains("QBRP1")) j.at("QBRP1").get_to(p.QBRP1);
    if (j.contains("QDRP1")) j.at("QDRP1").get_to(p.QDRP1);
    if (j.contains("PECP1")) j.at("PECP1").get_to(p.PECP1);
    if (j.contains("PECP2")) j.at("PECP2").get_to(p.PECP2);
    if (j.contains("FZ0")) j.at("FZ0").get_to(p.FZ0);
    if (j.contains("P0")) j.at("P0").get_to(p.P0);
    if (j.contains("VREF")) j.at("VREF").get_to(p.VREF);
    if (j.contains("R0")) j.at("R0").get_to(p.R0);
    if (j.contains("PRESMIN")) j.at("PRESMIN").get_to(p.PRESMIN);
    if (j.contains("PRESMAX")) j.at("PRESMAX").get_to(p.PRESMAX);
    if (j.contains("FZMIN")) j.at("FZMIN").get_to(p.FZMIN);
    if (j.contains("FZMAX")) j.at("FZMAX").get_to(p.FZMAX);
    if (j.contains("KPUMIN")) j.at("KPUMIN").get_to(p.KPUMIN);
    if (j.contains("KPUMAX")) j.at("KPUMAX").get_to(p.KPUMAX);
    if (j.contains("ALPMIN")) j.at("ALPMIN").get_to(p.ALPMIN);
    if (j.contains("ALPMAX")) j.at("ALPMAX").get_to(p.ALPMAX);
    if (j.contains("CAMMIN")) j.at("CAMMIN").get_to(p.CAMMIN);
    if (j.contains("CAMMAX")) j.at("CAMMAX").get_to(p.CAMMAX);
    if (j.contains("LFZO")) j.at("LFZO").get_to(p.LFZO);
}

void to_json(json& j, const MFContext& c)
{
    j["params"] = c.params;
    j["featureLevel"] = static_cast<int>(c.featureLevel);
    j["TurnSlip"] = c.TurnSlip;
    j["alpha_deg_f"] = c.alpha_deg_f;
    j["slip_ratio_f"] = c.slip_ratio_f;
    j["fz_f"] = c.fz_f;
    j["camber_deg_f"] = c.camber_deg_f;
    j["pressure_f"] = c.pressure_f;
    j["psi_f"] = c.psi_f;
    j["vx_f"] = c.vx_f;
    j["r_f"] = c.r_f;
}

void from_json(const json& j, MFContext& c)
{
    if (j.contains("params")) j.at("params").get_to(c.params);
    if (j.contains("featureLevel"))
    {
        int fl;
        j.at("featureLevel").get_to(fl);
        c.featureLevel = static_cast<MFFeatureLevel>(fl);
    }
    if (j.contains("TurnSlip")) j.at("TurnSlip").get_to(c.TurnSlip);
    if (j.contains("alpha_deg_f")) j.at("alpha_deg_f").get_to(c.alpha_deg_f);
    if (j.contains("slip_ratio_f")) j.at("slip_ratio_f").get_to(c.slip_ratio_f);
    if (j.contains("fz_f")) j.at("fz_f").get_to(c.fz_f);
    if (j.contains("camber_deg_f")) j.at("camber_deg_f").get_to(c.camber_deg_f);
    if (j.contains("pressure_f")) j.at("pressure_f").get_to(c.pressure_f);
    if (j.contains("psi_f")) j.at("psi_f").get_to(c.psi_f);
    if (j.contains("vx_f")) j.at("vx_f").get_to(c.vx_f);
    if (j.contains("r_f")) j.at("r_f").get_to(c.r_f);
}

void SaveTireConfig(const char* filename,
    const SimplePacejkaContext& simpleCtx,
    const MFContext& mfCtx,
    const ClassicPacejkaContext& classicCtx,
    const FialaContext& fialaCtx,
    const UAContext& uaCtx,
    const DugoffContext& dugoffCtx,
    const Tire521Context& tire521Ctx)
{
    json j;
    j["SimplePacejka"] = simpleCtx;
    j["MagicFormula"] = mfCtx;
    j["ClassicPacejka"] = classicCtx;
    j["Fiala"] = fialaCtx;
    j["UA"] = uaCtx;
    j["Dugoff"] = dugoffCtx;
    j["Tire521"] = tire521Ctx;

    std::ofstream o(filename);
    if (o.is_open())
    {
        o << j.dump(4) << std::endl;
        std::cout << "Configuration saved to: " << filename << std::endl;
    }
    else
    {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
    }
}

void LoadTireConfig(const char* filename,
    SimplePacejkaContext& simpleCtx,
    MFContext& mfCtx,
    ClassicPacejkaContext& classicCtx,
    FialaContext& fialaCtx,
    UAContext& uaCtx,
    DugoffContext& dugoffCtx,
    Tire521Context& tire521Ctx)
{
    std::ifstream i(filename);
    if (!i.is_open())
    {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    json j;
    try
    {
        i >> j;
        if (j.contains("SimplePacejka")) j.at("SimplePacejka").get_to(simpleCtx);
        if (j.contains("MagicFormula")) j.at("MagicFormula").get_to(mfCtx);
        if (j.contains("ClassicPacejka")) j.at("ClassicPacejka").get_to(classicCtx);
        if (j.contains("Fiala")) j.at("Fiala").get_to(fialaCtx);
        if (j.contains("UA")) j.at("UA").get_to(uaCtx);
        if (j.contains("Dugoff")) j.at("Dugoff").get_to(dugoffCtx);
        if (j.contains("Tire521")) j.at("Tire521").get_to(tire521Ctx);
        std::cout << "Configuration loaded from: " << filename << std::endl;
    }
    catch (const json::exception& e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}