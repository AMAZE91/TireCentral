// TireCentral.cpp : Defines the entry point for the application.
//

#include "TireCentral.h"
#include "imgui.h"
#include "implot.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "Serialization.h"

using namespace std;

namespace UIConfig
{
    constexpr float StepButtonWidth = 28.0f;
    constexpr float InputWidth = 130.0f;
    constexpr ImVec2 ButtonPadding = ImVec2(6.0f, 2.0f);
    constexpr float Spacing = 4.0f;

    const ImVec4 PlotColorBase = ImVec4(0.26f, 0.59f, 0.98f, 0.4);
    const ImVec4 PlotColorCurrent = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
    const ImVec4 PlotColorFzDelta1 = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
    const ImVec4 PlotColorFzDelta2 = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
    const ImVec4 PlotColorFzDelta3 = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
    const ImVec4 PlotColorFzDelta4 = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);

    constexpr int MaxFzDeltas = 4;
    static float FzDeltas[MaxFzDeltas] = { 1000.0f, 2000.0f, 3000.0f, 4000.0f };
    static int NumFzDeltas = 2;
    static float FzDeltaStep = 500.0f;

    inline void RenderFzDeltaConfig()
    {
        if (ImGui::CollapsingHeader("Fz Variation Plot Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SliderInt("Number of Delta Lines", &NumFzDeltas, 0, MaxFzDeltas);
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Configure additional Fz curves:");

            for (int i{}; i < NumFzDeltas; i++)
            {
                char label[32];
                snprintf(label, sizeof(label), "Delta %d [N]", i + 1);
                ImGui::PushID(i);
                const ImVec4* color = (i == 0) ? &PlotColorFzDelta1 : (i == 1) ? &PlotColorFzDelta2 : (i == 2) ? &PlotColorFzDelta3 : &PlotColorFzDelta4;
                ImGui::PushStyleColor(ImGuiCol_Text, *color);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(color->x, color->y, color->z, 0.2f));
                ImGui::InputFloat(label, &FzDeltas[i], FzDeltaStep, FzDeltaStep * 10.0f, "%.0f");
                ImGui::PopStyleColor(2);
                ImGui::PopID();
            }
        }
    }
}

#define STEP_F(name, var, step, format) \
    do \
    { \
        ImGui::Text("%s", name); \
        ImGui::NewLine(); \
        ImGui::PushID(#var); \
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, UIConfig::ButtonPadding); \
        if (ImGui::Button("<", ImVec2(UIConfig::StepButtonWidth, 0))) params.var -= (step); \
        ImGui::PopStyleVar(); \
        ImGui::SameLine(0, UIConfig::Spacing); \
        ImGui::PushItemWidth(UIConfig::InputWidth); \
        ImGui::InputDouble("##val", &params.var, (step), (step) * 10.0, (format)); \
        ImGui::PopItemWidth(); \
        ImGui::SameLine(0, UIConfig::Spacing); \
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, UIConfig::ButtonPadding); \
        if (ImGui::Button(">", ImVec2(UIConfig::StepButtonWidth, 0))) params.var += (step); \
        ImGui::PopStyleVar(); \
        ImGui::PopID(); \
    } while(0)

void PlotPureForceFzVariations(
    const char* title, const char* x_label, const char* y_label,
    float x_min, float x_max, TMState base_state,
    std::function<void(TMState&, prec)> state_modifier,
    std::function<TMOutput(const TMState&)> model_eval,
    std::function<prec(const TMOutput&)> force_extractor,
    prec current_x, prec current_force, int N = 200
)
{
    if (ImPlot::BeginPlot(title, ImVec2(-1.0f, -1.0f)))
    {
        ImPlot::SetupAxes(x_label, y_label);

        std::vector<prec> x_data(N), y_base(N);
        std::vector<std::vector<prec>> y_deltas(UIConfig::NumFzDeltas, std::vector<prec>(N));
        prec max_f{};

        for (int i{}; i < N; i++)
        {
            prec x_val = x_min + (x_max - x_min) * i / (N - 1);
            x_data[i] = x_val;

            TMState ts = base_state;
            state_modifier(ts, x_val);

            TMOutput out = model_eval(ts);
            y_base[i] = force_extractor(out);
            max_f = std::max(max_f, std::abs(y_base[i]));

            for (int d{}; d < UIConfig::NumFzDeltas; d++)
            {
                TMState ts_delta = ts;
                ts_delta.Fz = base_state.Fz + UIConfig::FzDeltas[d];
                TMOutput out_delta = model_eval(ts_delta);
                y_deltas[d][i] = force_extractor(out_delta);
                max_f = std::max(max_f, std::abs(y_deltas[d][i]));
            }
        }

        prec y_pad = max_f * 0.1;
        if (y_pad < 100) y_pad = 100;
        ImPlot::SetupAxesLimits(x_min, x_max, -max_f - y_pad, max_f + y_pad, ImGuiCond_Always);

        ImPlotSpec specBase;
        specBase.LineWeight = 3.0f;
        ImPlot::PushStyleColor(ImPlotProp_LineColor, UIConfig::PlotColorBase);
        ImPlot::PlotLine("Base Fz", x_data.data(), y_base.data(), N, specBase);
        ImPlot::PopStyleColor();

        const ImVec4 delta_colors[] = { UIConfig::PlotColorFzDelta1, UIConfig::PlotColorFzDelta2, UIConfig::PlotColorFzDelta3, UIConfig::PlotColorFzDelta4 };
        for (int d{}; d < UIConfig::NumFzDeltas; d++)
        {
            ImPlotSpec specDelta;
            specDelta.LineWeight = 2.0f;
            char label[64];
            snprintf(label, sizeof(label), "Fz + %.0f N", UIConfig::FzDeltas[d]);
            ImPlot::PushStyleColor(ImPlotProp_LineColor, delta_colors[d]);
            ImPlot::PlotLine(label, x_data.data(), y_deltas[d].data(), N, specDelta);
            ImPlot::PopStyleColor();
        }

        ImPlotSpec ptSpec;
        ptSpec.Marker = ImPlotMarker_Circle;
        ptSpec.MarkerSize = 8.0f;
        ptSpec.MarkerFillColor = UIConfig::PlotColorCurrent;
        ImPlot::PlotScatter("Current", &current_x, &current_force, 1, ptSpec);
        ImPlot::EndPlot();
    }
}

void PlotCombinedVsPure(
    const char* title, const char* x_label, const char* y_label,
    float x_min, float x_max, TMState base_state,
    std::function<void(TMState&, prec)> state_modifier,
    std::function<TMOutput(const TMState&)> model_eval,
    std::function<prec(const TMOutput&)> pure_extractor,
    std::function<prec(const TMOutput&)> comb_extractor, int N = 200
)
{
    if (ImPlot::BeginPlot(title, ImVec2(-1.0f, -1.0f)))
    {
        ImPlot::SetupAxes(x_label, y_label);
        std::vector<prec> x_data(N), y_pure(N), y_comb(N);
        prec max_f{};
        for (int i{}; i < N; i++)
        {
            prec x = x_min + (x_max - x_min) * i / (N - 1);
            x_data[i] = x;
            TMState ts = base_state;
            state_modifier(ts, x);
            TMOutput out = model_eval(ts);
            y_pure[i] = pure_extractor(out);
            y_comb[i] = comb_extractor(out);
            max_f = std::max({ max_f, std::abs(y_pure[i]), std::abs(y_comb[i]) });
        }
        prec y_pad = max_f * 0.1;
        if (y_pad < 100) y_pad = 100;
        ImPlot::SetupAxesLimits(x_min, x_max, -max_f - y_pad, max_f + y_pad, ImGuiCond_Always);

        ImPlotSpec specPure;
        specPure.LineWeight = 3.0f;
        ImPlot::PushStyleColor(ImPlotProp_LineColor, UIConfig::PlotColorBase);
        ImPlot::PlotLine("Pure", x_data.data(), y_pure.data(), N, specPure);
        ImPlot::PopStyleColor();

        ImPlotSpec specComb;
        specComb.LineWeight = 2.0f;
        ImPlot::PushStyleColor(ImPlotProp_LineColor, UIConfig::PlotColorCurrent);
        ImPlot::PlotLine("Combined", x_data.data(), y_comb.data(), N, specComb);
        ImPlot::PopStyleColor();
        ImPlot::EndPlot();
    }
}

void PlotMoment(
    const char* title, const char* x_label, const char* y_label,
    float x_min, float x_max, TMState base_state,
    std::function<void(TMState&, prec)> state_modifier,
    std::function<TMOutput(const TMState&)> model_eval,
    std::function<prec(const TMOutput&)> moment_extractor,
    prec current_x, prec current_moment, int N = 200
)
{
    if (ImPlot::BeginPlot(title, ImVec2(-1.0f, -1.0f)))
    {
        ImPlot::SetupAxes(x_label, y_label);
        std::vector<prec> x_data(N), y_data(N);
        prec max_m{};
        for (int i{}; i < N; i++)
        {
            prec x = x_min + (x_max - x_min) * i / (N - 1);
            x_data[i] = x;
            TMState ts = base_state;
            state_modifier(ts, x);
            TMOutput out = model_eval(ts);
            y_data[i] = moment_extractor(out);
            max_m = std::max(max_m, std::abs(y_data[i]));
        }
        prec pad = max_m * 0.1;
        if (pad < 1.0) pad = 1.0;
        ImPlot::SetupAxesLimits(x_min, x_max, -max_m - pad, max_m + pad, ImGuiCond_Always);

        ImPlotSpec spec;
        spec.LineWeight = 3.0f;
        ImPlot::PushStyleColor(ImPlotProp_LineColor, UIConfig::PlotColorBase);
        ImPlot::PlotLine("Curve", x_data.data(), y_data.data(), N, spec);
        ImPlot::PopStyleColor();

        ImPlotSpec ptSpec;
        ptSpec.Marker = ImPlotMarker_Circle;
        ptSpec.MarkerSize = 8.0f;
        ptSpec.MarkerFillColor = UIConfig::PlotColorCurrent;
        ImPlot::PlotScatter("Current", &current_x, &current_moment, 1, ptSpec);
        ImPlot::EndPlot();
    }
}

void RenderSimplePacejkaConfig(SimplePacejkaContext& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");
        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
    }

    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("Pacejka Parameters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Longitudinal (Fx)");
        STEP_F("Bx", Bx, 0.1, "%.2f");
        STEP_F("Cx", Cx, 0.01, "%.2f");
        STEP_F("Dx", Dx, 0.01, "%.2f");
        STEP_F("Ex", Ex, 0.1, "%.2f");
        ImGui::Spacing();
        ImGui::Text("Lateral (Fy)");
        STEP_F("By", By, 0.1, "%.2f");
        STEP_F("Cy", Cy, 0.01, "%.2f");
        STEP_F("Dy", Dy, 0.01, "%.2f");
        STEP_F("Ey", Ey, 0.1, "%.2f");
    }

    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0);
    ImGui::Text("Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Slip Ratio: %.3f | Slip Angle: %.2f deg | Sc: %.3f", ctx.slip_ratio_f, ctx.alpha_deg_f, ctx.current_out.Sc);
}

void RenderSimplePacejkaGraphs(const SimplePacejkaContext& ctx)
{
    if (ImGui::BeginTabBar("SimpleTabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f,
                ctx.state, [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return SimplePacejka(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f,
                ctx.state, [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return SimplePacejka(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return SimplePacejka(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return SimplePacejka(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Friction Circle (Sc)"))
        {
            if (ImPlot::BeginPlot("Normalized Friction Circle", ImVec2(-1.0f, -1.0f)))
            {
                ImPlot::SetupAxes("RatioX (Fx0 / MaxFx)", "RatioY (Fy0 / MaxFy)");
                ImPlot::SetupAxesLimits(-1.5, 1.5, -1.5, 1.5, ImGuiCond_Always);
                const int N_circ = 100;
                std::vector<prec> cx(N_circ), cy(N_circ);
                for (int i = 0; i < N_circ; i++)
                {
                    prec theta = 2.0 * std::numbers::pi_v<prec> *i / (N_circ - 1);
                    cx[i] = std::cos(theta);
                    cy[i] = std::sin(theta);
                }
                ImPlotSpec circSpec;
                circSpec.LineWeight = 3.0f;
                ImPlot::PushStyleColor(ImPlotProp_LineColor, ImVec4(1, 1, 1, 0.5f));
                ImPlot::PlotLine("Limit (Sc=1)", cx.data(), cy.data(), N_circ, circSpec);
                ImPlot::PopStyleColor();

                prec MaxFx = ctx.state.Fz * ctx.params.Dx;
                prec MaxFy = ctx.state.Fz * ctx.params.Dy;
                prec RatioX = (MaxFx != 0) ? ctx.current_out.Fx0 / MaxFx : 0;
                prec RatioY = (MaxFy != 0) ? ctx.current_out.Fy0 / MaxFy : 0;
                prec CombRatioX = (MaxFx != 0) ? ctx.current_out.Fx / MaxFx : 0;
                prec CombRatioY = (MaxFy != 0) ? ctx.current_out.Fy / MaxFy : 0;

                ImPlotSpec pureSpec;
                pureSpec.Marker = ImPlotMarker_Square;
                pureSpec.MarkerSize = 10.0f;
                pureSpec.MarkerFillColor = ImVec4(1, 0.5f, 0, 1);
                ImPlot::PlotScatter("Pure", &RatioX, &RatioY, 1, pureSpec);
                ImPlotSpec combSpec;
                combSpec.Marker = ImPlotMarker_Circle;
                combSpec.MarkerSize = 10.0f;
                combSpec.MarkerFillColor = ImVec4(0, 1, 1, 1);
                ImPlot::PlotScatter("Combined", &CombRatioX, &CombRatioY, 1, combSpec);
                ImPlot::EndPlot();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void RenderMFConfig(MFContext& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Inclination (Camber) [deg]", &ctx.camber_deg_f, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Pressure [Pa]", &ctx.pressure_f, 100000.0f, 400000.0f, "%.0f");
        ImGui::SliderFloat("Turn Slip (Psi)", &ctx.psi_f, -1.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Velocity (Vx) [m/s]", &ctx.vx_f, 0.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Radius (R) [m]", &ctx.r_f, 0.1f, 0.5f, "%.2f");

        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
        ctx.state.InclinationAngle = ctx.camber_deg_f / HLP::R2D;
        ctx.state.Pressure = ctx.pressure_f;
        ctx.state.Psi = ctx.psi_f;
        ctx.state.Vx = ctx.vx_f;
        ctx.state.R = ctx.r_f;
    }

    int currentFeatureLevel = static_cast<int>(ctx.featureLevel);
    if (ImGui::Combo("Feature Level", &currentFeatureLevel, "MF 6.2\0MC Tyre 1.1\0MF 5.2\0MF 6.0\0"))
    {
        ctx.featureLevel = static_cast<MFFeatureLevel>(currentFeatureLevel);
    }
    int currentTurnSlipLevel = static_cast<int>(ctx.TurnSlip);
    if (ImGui::Combo("Turn Slip", &currentTurnSlipLevel, "Disabled\0Enabled\0"))
    {
        ctx.TurnSlip = static_cast<bool>(currentTurnSlipLevel);
    }

    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("MF Parameters"))
    {
        if (ImGui::TreeNode("Scaling Factors"))
        {
            STEP_F("LCX", LCX, 0.01, "%.3f");
            STEP_F("LMUX", LMUX, 0.01, "%.3f");
            STEP_F("LEX", LEX, 0.01, "%.3f");
            STEP_F("LKX", LKX, 0.01, "%.3f");
            STEP_F("LHX", LHX, 0.01, "%.3f");
            STEP_F("LVX", LVX, 0.01, "%.3f");
            STEP_F("LXAL", LXAL, 0.01, "%.3f");
            ImGui::Separator();
            STEP_F("LCY", LCY, 0.01, "%.3f");
            STEP_F("LMUY", LMUY, 0.01, "%.3f");
            STEP_F("LEY", LEY, 0.01, "%.3f");
            STEP_F("LKY", LKY, 0.01, "%.3f");
            STEP_F("LKYC", LKYC, 0.01, "%.3f");
            STEP_F("LKZC", LKZC, 0.01, "%.3f");
            STEP_F("LHY", LHY, 0.01, "%.3f");
            STEP_F("LVY", LVY, 0.01, "%.3f");
            STEP_F("LYKA", LYKA, 0.01, "%.3f");
            STEP_F("LVYKA", LVYKA, 0.01, "%.3f");
            STEP_F("LKC", LKC, 0.01, "%.3f");
            STEP_F("LCC", LCC, 0.01, "%.3f");
            STEP_F("LEC", LEC, 0.01, "%.3f");
            ImGui::Separator();
            STEP_F("LMX", LMX, 0.01, "%.3f");
            STEP_F("LVMX", LVMX, 0.01, "%.3f");
            STEP_F("LMY", LMY, 0.01, "%.3f");
            STEP_F("LTR", LTR, 0.01, "%.3f");
            STEP_F("LRES", LRES, 0.01, "%.3f");
            STEP_F("LS", LS, 0.01, "%.3f");
            STEP_F("LMP", LMP, 0.01, "%.3f");
            STEP_F("LGYR", LGYR, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("General"))
        {
            STEP_F("FZ0", FZ0, 100.0, "%.0f");
            STEP_F("P0", P0, 1000.0, "%.0f");
            STEP_F("VREF", VREF, 1.0, "%.1f");
            STEP_F("R0", R0, 0.01, "%.3f");
            ImGui::Separator();
            STEP_F("PRESMIN", PRESMIN, 1000.0, "%.0f");
            STEP_F("PRESMAX", PRESMAX, 1000.0, "%.0f");
            STEP_F("FZMIN", FZMIN, 100.0, "%.0f");
            STEP_F("FZMAX", FZMAX, 100.0, "%.0f");
            ImGui::Separator();
            STEP_F("KPUMIN", KPUMIN, 0.1, "%.2f");
            STEP_F("KPUMAX", KPUMAX, 0.1, "%.2f");
            STEP_F("ALPMIN", ALPMIN, 0.01, "%.3f");
            STEP_F("ALPMAX", ALPMAX, 0.01, "%.3f");
            STEP_F("CAMMIN", CAMMIN, 0.01, "%.3f");
            STEP_F("CAMMAX", CAMMAX, 0.01, "%.3f");
            STEP_F("LFZO", LFZO, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Longitudinal Pure"))
        {
            STEP_F("PCX1", PCX1, 0.01, "%.3f");
            STEP_F("PDX1", PDX1, 0.01, "%.3f");
            STEP_F("PDX2", PDX2, 0.01, "%.3f");
            STEP_F("PDX3", PDX3, 0.01, "%.3f");
            STEP_F("PEX1", PEX1, 0.01, "%.3f");
            STEP_F("PEX2", PEX2, 0.01, "%.3f");
            STEP_F("PEX3", PEX3, 0.01, "%.3f");
            STEP_F("PEX4", PEX4, 0.01, "%.3f");
            STEP_F("PKX1", PKX1, 1.0, "%.1f");
            STEP_F("PKX2", PKX2, 0.1, "%.2f");
            STEP_F("PKX3", PKX3, 0.01, "%.3f");
            STEP_F("PHX1", PHX1, 0.01, "%.3f");
            STEP_F("PHX2", PHX2, 0.01, "%.3f");
            STEP_F("PVX1", PVX1, 0.01, "%.3f");
            STEP_F("PVX2", PVX2, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Longitudinal Combined"))
        {
            STEP_F("RBX1", RBX1, 0.1, "%.2f");
            STEP_F("RBX2", RBX2, 0.1, "%.2f");
            STEP_F("RBX3", RBX3, 0.01, "%.3f");
            STEP_F("RCX1", RCX1, 0.01, "%.3f");
            STEP_F("REX1", REX1, 0.01, "%.3f");
            STEP_F("REX2", REX2, 0.01, "%.3f");
            STEP_F("RHX1", RHX1, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Pressure & Spin Fx"))
        {
            STEP_F("PPX1", PPX1, 0.01, "%.3f");
            STEP_F("PPX2", PPX2, 0.01, "%.3f");
            STEP_F("PPX3", PPX3, 0.01, "%.3f");
            STEP_F("PPX4", PPX4, 0.01, "%.3f");
            STEP_F("PDXP1", PDXP1, 0.01, "%.3f");
            STEP_F("PDXP2", PDXP2, 0.01, "%.3f");
            STEP_F("PDXP3", PDXP3, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Lateral Pure"))
        {
            STEP_F("PCY1", PCY1, 0.01, "%.3f");
            STEP_F("PCY2", PCY2, 0.01, "%.3f");
            STEP_F("PDY1", PDY1, 0.01, "%.3f");
            STEP_F("PDY2", PDY2, 0.01, "%.3f");
            STEP_F("PDY3", PDY3, 0.01, "%.3f");
            STEP_F("PEY1", PEY1, 0.01, "%.3f");
            STEP_F("PEY2", PEY2, 0.01, "%.3f");
            STEP_F("PEY3", PEY3, 0.01, "%.3f");
            STEP_F("PEY4", PEY4, 0.01, "%.3f");
            STEP_F("PEY5", PEY5, 0.01, "%.3f");
            STEP_F("PKY1", PKY1, 1.0, "%.1f");
            STEP_F("PKY2", PKY2, 0.1, "%.2f");
            STEP_F("PKY3", PKY3, 0.01, "%.3f");
            STEP_F("PKY4", PKY4, 0.01, "%.3f");
            STEP_F("PKY5", PKY5, 0.01, "%.3f");
            STEP_F("PKY6", PKY6, 0.01, "%.3f");
            STEP_F("PKY7", PKY7, 0.01, "%.3f");
            STEP_F("PHY1", PHY1, 0.01, "%.3f");
            STEP_F("PHY2", PHY2, 0.01, "%.3f");
            STEP_F("PHY3", PHY3, 0.01, "%.3f");
            STEP_F("PVY1", PVY1, 0.01, "%.3f");
            STEP_F("PVY2", PVY2, 0.01, "%.3f");
            STEP_F("PVY3", PVY3, 0.01, "%.3f");
            STEP_F("PVY4", PVY4, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Lateral Combined"))
        {
            STEP_F("RBY1", RBY1, 0.1, "%.2f");
            STEP_F("RBY2", RBY2, 0.1, "%.2f");
            STEP_F("RBY3", RBY3, 0.01, "%.3f");
            STEP_F("RBY4", RBY4, 0.01, "%.3f");
            STEP_F("RCY1", RCY1, 0.01, "%.3f");
            STEP_F("REY1", REY1, 0.01, "%.3f");
            STEP_F("REY2", REY2, 0.01, "%.3f");
            STEP_F("RHY1", RHY1, 0.01, "%.3f");
            STEP_F("RHY2", RHY2, 0.01, "%.3f");
            STEP_F("RVY1", RVY1, 0.001, "%.4f");
            STEP_F("RVY2", RVY2, 0.001, "%.4f");
            STEP_F("RVY3", RVY3, 0.001, "%.4f");
            STEP_F("RVY4", RVY4, 0.001, "%.4f");
            STEP_F("RVY5", RVY5, 0.001, "%.4f");
            STEP_F("RVY6", RVY6, 0.001, "%.4f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Pressure & Spin Fy"))
        {
            STEP_F("PPY1", PPY1, 0.01, "%.3f");
            STEP_F("PPY2", PPY2, 0.01, "%.3f");
            STEP_F("PPY3", PPY3, 0.01, "%.3f");
            STEP_F("PPY4", PPY4, 0.01, "%.3f");
            STEP_F("PPY5", PPY5, 0.01, "%.3f");
            STEP_F("PKYP1", PKYP1, 0.01, "%.3f");
            STEP_F("PDYP1", PDYP1, 0.01, "%.3f");
            STEP_F("PDYP2", PDYP2, 0.01, "%.3f");
            STEP_F("PDYP3", PDYP3, 0.01, "%.3f");
            STEP_F("PDYP4", PDYP4, 0.01, "%.3f");
            STEP_F("PHYP1", PHYP1, 0.01, "%.3f");
            STEP_F("PHYP2", PHYP2, 0.01, "%.3f");
            STEP_F("PHYP3", PHYP3, 0.01, "%.3f");
            STEP_F("PHYP4", PHYP4, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Overturning Moment (Mx)"))
        {
            STEP_F("QSX1", QSX1, 0.001, "%.4f");
            STEP_F("QSX2", QSX2, 0.001, "%.4f");
            STEP_F("QSX3", QSX3, 0.001, "%.4f");
            STEP_F("QSX4", QSX4, 0.001, "%.4f");
            STEP_F("QSX5", QSX5, 0.001, "%.4f");
            STEP_F("QSX6", QSX6, 0.01, "%.3f");
            STEP_F("QSX7", QSX7, 0.001, "%.4f");
            STEP_F("QSX8", QSX8, 0.001, "%.4f");
            STEP_F("QSX9", QSX9, 0.001, "%.4f");
            STEP_F("QSX10", QSX10, 0.001, "%.4f");
            STEP_F("QSX11", QSX11, 0.001, "%.4f");
            STEP_F("QSX12", QSX12, 0.001, "%.4f");
            STEP_F("QSX13", QSX13, 0.001, "%.4f");
            STEP_F("QSX14", QSX14, 0.001, "%.4f");
            STEP_F("PPMX1", PPMX1, 0.001, "%.4f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Rolling Resistance (My)"))
        {
            STEP_F("QSY1", QSY1, 0.001, "%.4f");
            STEP_F("QSY2", QSY2, 0.001, "%.4f");
            STEP_F("QSY3", QSY3, 0.001, "%.4f");
            STEP_F("QSY4", QSY4, 0.00001, "%.6f");
            STEP_F("QSY5", QSY5, 0.0001, "%.5f");
            STEP_F("QSY6", QSY6, 0.0001, "%.5f");
            STEP_F("QSY7", QSY7, 0.01, "%.3f");
            STEP_F("QSY8", QSY8, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Aligning Torque (Mz)"))
        {
            STEP_F("QBZ1", QBZ1, 0.1, "%.2f");
            STEP_F("QBZ2", QBZ2, 0.01, "%.3f");
            STEP_F("QBZ3", QBZ3, 0.01, "%.3f");
            STEP_F("QBZ4", QBZ4, 0.001, "%.4f");
            STEP_F("QBZ5", QBZ5, 0.001, "%.4f");
            STEP_F("QBZ9", QBZ9, 0.1, "%.2f");
            STEP_F("QBZ10", QBZ10, 0.01, "%.3f");
            STEP_F("QCZ1", QCZ1, 0.01, "%.3f");
            STEP_F("QDZ1", QDZ1, 0.001, "%.4f");
            STEP_F("QDZ2", QDZ2, 0.001, "%.4f");
            STEP_F("QDZ3", QDZ3, 0.001, "%.4f");
            STEP_F("QDZ4", QDZ4, 0.001, "%.4f");
            STEP_F("QDZ6", QDZ6, 0.001, "%.4f");
            STEP_F("QDZ7", QDZ7, 0.001, "%.4f");
            STEP_F("QDZ8", QDZ8, 0.001, "%.4f");
            STEP_F("QDZ9", QDZ9, 0.001, "%.4f");
            STEP_F("QDZ10", QDZ10, 0.001, "%.4f");
            STEP_F("QDZ11", QDZ11, 0.001, "%.4f");
            STEP_F("QEZ1", QEZ1, 0.01, "%.3f");
            STEP_F("QEZ2", QEZ2, 0.01, "%.3f");
            STEP_F("QEZ3", QEZ3, 0.01, "%.3f");
            STEP_F("QEZ4", QEZ4, 0.01, "%.3f");
            STEP_F("QEZ5", QEZ5, 0.01, "%.3f");
            STEP_F("QHZ1", QHZ1, 0.01, "%.3f");
            STEP_F("QHZ2", QHZ2, 0.01, "%.3f");
            STEP_F("QHZ3", QHZ3, 0.01, "%.3f");
            STEP_F("QHZ4", QHZ4, 0.01, "%.3f");
            STEP_F("SSZ1", SSZ1, 0.0001, "%.5f");
            STEP_F("SSZ2", SSZ2, 0.0001, "%.5f");
            STEP_F("SSZ3", SSZ3, 0.0001, "%.5f");
            STEP_F("SSZ4", SSZ4, 0.0001, "%.5f");
            STEP_F("PPZ1", PPZ1, 0.01, "%.3f");
            STEP_F("PPZ2", PPZ2, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Turn Slip & Spin"))
        {
            STEP_F("QDTP1", QDTP1, 0.001, "%.4f");
            STEP_F("QCRP1", QCRP1, 0.001, "%.4f");
            STEP_F("QCRP2", QCRP2, 0.001, "%.4f");
            STEP_F("QBRP1", QBRP1, 0.001, "%.4f");
            STEP_F("QDRP1", QDRP1, 0.001, "%.4f");
            STEP_F("PECP1", PECP1, 0.01, "%.3f");
            STEP_F("PECP2", PECP2, 0.01, "%.3f");
            ImGui::TreePop();
        }
    }

    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f) | Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0, ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Mx: %.2f Nm | My: %.2f Nm | Mz: %.2f Nm", ctx.current_out.Mx, ctx.current_out.My, ctx.current_out.Mz);
    ImGui::Text("Gxa: %.3f | Gyk: %.3f", ctx.current_out.Gxa, ctx.current_out.Gyk);
}

void RenderMFGraphs(const MFContext& ctx)
{
    if (ImGui::BeginTabBar("MFTabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mx (Overturning)"))
        {
            PlotMoment("Overturning Moment (Mx)", "Slip Angle [deg]", "Mx [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.Mx; }, ctx.alpha_deg_f, ctx.current_out.Mx);
            ImGui::Text("Current Mx: %.2f Nm", ctx.current_out.Mx);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("My (Rolling Resistance)"))
        {
            PlotMoment("Rolling Resistance Moment (My)", "Velocity (Vx) [m/s]", "My [Nm]", 0.0f, 50.0f, ctx.state,
                [](TMState& ts, prec x) { ts.Vx = x; }, [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.My; }, ctx.vx_f, ctx.current_out.My);
            ImGui::Text("Current My: %.2f Nm", ctx.current_out.My);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mz (Aligning Torque)"))
        {
            PlotMoment("Aligning Torque (Mz)", "Slip Angle [deg]", "Mz [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip); },
                [](const TMOutput& out) { return out.Mz; }, ctx.alpha_deg_f, ctx.current_out.Mz);
            ImGui::Text("Current Mz: %.2f Nm", ctx.current_out.Mz);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Force Envelope (MF vs Ellipse)"))
        {
            static std::vector<prec> fx_pts, fy_pts, ell_x, ell_y;
            fx_pts.clear();
            fy_pts.clear();
            ell_x.clear();
            ell_y.clear();
            const int N_kappa = 50, N_alpha = 50;
            prec fx_max{}, fy_max{};
            for (int i = 0; i < N_kappa; i++)
            {
                prec k = -0.3 + 0.6 * i / (N_kappa - 1);
                for (int j = 0; j < N_alpha; j++)
                {
                    prec a_deg = -15.0 + 30.0 * j / (N_alpha - 1);
                    TMState ts = ctx.state;
                    ts.SlipRatio = k;
                    ts.SlipAngle = a_deg / HLP::R2D;
                    TMOutput out = MF(ctx.params, ts, ctx.featureLevel, ctx.TurnSlip);
                    fx_pts.push_back(out.Fx);
                    fy_pts.push_back(out.Fy);
                    fx_max = std::max(fx_max, std::abs(out.Fx));
                    fy_max = std::max(fy_max, std::abs(out.Fy));
                }
            }
            const int N_ell = 100;
            for (int i = 0; i < N_ell; i++)
            {
                prec theta = 2.0 * std::numbers::pi_v<prec> *i / (N_ell - 1);
                ell_x.push_back(fy_max * std::cos(theta));
                ell_y.push_back(fx_max * std::sin(theta));
            }
            if (ImPlot::BeginPlot("Tire Force Envelope", ImVec2(-1.0f, -1.0f)))
            {
                ImPlot::SetupAxes("Lateral Force Fy [N]", "Longitudinal Force Fx [N]");
                prec pad_x = fy_max * 0.1;
                if (pad_x < 100) pad_x = 100;
                prec pad_y = fx_max * 0.1;
                if (pad_y < 100) pad_y = 100;
                ImPlot::SetupAxesLimits(-fy_max - pad_x, fy_max + pad_x, -fx_max - pad_y, fx_max + pad_y, ImGuiCond_Always);
                ImPlotSpec scatterSpec;
                scatterSpec.Marker = ImPlotMarker_Circle;
                scatterSpec.MarkerSize = 2.0f;
                scatterSpec.MarkerFillColor = ImVec4(0.2f, 0.8f, 0.8f, 0.6f);
                ImPlot::PlotScatter("MF Grid", fy_pts.data(), fx_pts.data(), fx_pts.size(), scatterSpec);
                ImPlotSpec ellSpec;
                ellSpec.LineWeight = 3.0f;
                ImPlot::PushStyleColor(ImPlotProp_LineColor, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImPlot::PlotLine("Reference Ellipse", ell_x.data(), ell_y.data(), N_ell, ellSpec);
                ImPlot::PopStyleColor();
                ImPlotSpec ptSpec;
                ptSpec.Marker = ImPlotMarker_Circle;
                ptSpec.MarkerSize = 10.0f;
                ptSpec.MarkerFillColor = ImVec4(1.0, 1.0f, 0.0f, 1.0f);
                ptSpec.MarkerLineColor = ImVec4(0, 0, 0, 1);
                ImPlot::PlotScatter("Current", &ctx.current_out.Fy, &ctx.current_out.Fx, 1, ptSpec);
                ImPlot::EndPlot();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void RenderClassicPacejkaConfig(ClassicPacejkaContext& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Inclination (Camber) [deg]", &ctx.camber_deg_f, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Velocity (Vx) [m/s]", &ctx.vx_f, 0.0f, 100.0f, "%.1f");
        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
        ctx.state.InclinationAngle = ctx.camber_deg_f / HLP::R2D;
        ctx.state.Vx = ctx.vx_f;
    }
    int currentFeatureLevel = static_cast<int>(ctx.featureLevel);
    if (ImGui::Combo("Feature Level", &currentFeatureLevel, "Pacejka 89\0Pacejka 94\0"))
    {
        ctx.featureLevel = static_cast<PacejkaFeatureLevel>(currentFeatureLevel);
    }
    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("Pacejka Parameters"))
    {
        if (ImGui::TreeNode("Scaling Factors"))
        {
            STEP_F("DLON", DLON, 0.01, "%.3f");
            STEP_F("DLAT", DLAT, 0.01, "%.3f");
            STEP_F("BCDLON", BCDLON, 0.01, "%.3f");
            STEP_F("BCDLAT", BCDLAT, 0.01, "%.3f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Longitudinal (Fx)"))
        {
            STEP_F("B0", B0, 0.01, "%.3f");
            STEP_F("B1", B1, 1.0, "%.1f");
            STEP_F("B2", B2, 10.0, "%.1f");
            STEP_F("B3", B3, 1.0, "%.1f");
            STEP_F("B4", B4, 10.0, "%.1f");
            STEP_F("B5", B5, 0.1, "%.2f");
            STEP_F("B6", B6, 0.01, "%.3f");
            STEP_F("B7", B7, 0.1, "%.2f");
            STEP_F("B8", B8, 1.0, "%.1f");
            STEP_F("B9", B9, 0.1, "%.2f");
            STEP_F("B10", B10, 0.1, "%.2f");
            STEP_F("B11", B11, 10.0, "%.1f");
            STEP_F("B12", B12, 1.0, "%.1f");
            STEP_F("B13", B13, 0.1, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Lateral (Fy)"))
        {
            STEP_F("A0", A0, 0.01, "%.3f");
            STEP_F("A1", A1, 1.0, "%.1f");
            STEP_F("A2", A2, 10.0, "%.1f");
            STEP_F("A3", A3, 10.0, "%.1f");
            STEP_F("A4", A4, 1.0, "%.1f");
            STEP_F("A5", A5, 0.01, "%.3f");
            STEP_F("A6", A6, 0.1, "%.2f");
            STEP_F("A7", A7, 1.0, "%.1f");
            STEP_F("A8", A8, 0.1, "%.2f");
            STEP_F("A9", A9, 0.1, "%.2f");
            STEP_F("A10", A10, 0.01, "%.3f");
            STEP_F("A11", A11, 10.0, "%.1f");
            STEP_F("A12", A12, 1.0, "%.1f");
            STEP_F("A13", A13, 1.0, "%.1f");
            STEP_F("A14", A14, 1.0, "%.1f");
            STEP_F("A15", A15, 0.001, "%.4f");
            STEP_F("A16", A16, 0.01, "%.3f");
            STEP_F("A17", A17, 0.1, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Aligning Torque (Mz)"))
        {
            STEP_F("C0", C0, 0.01, "%.3f");
            STEP_F("C1", C1, 0.1, "%.2f");
            STEP_F("C2", C2, 0.1, "%.2f");
            STEP_F("C3", C3, 1.0, "%.1f");
            STEP_F("C4", C4, 1.0, "%.1f");
            STEP_F("C5", C5, 0.1, "%.2f");
            STEP_F("C6", C6, 0.1, "%.2f");
            STEP_F("C7", C7, 0.1, "%.2f");
            STEP_F("C8", C8, 0.1, "%.2f");
            STEP_F("C9", C9, 0.1, "%.2f");
            STEP_F("C10", C10, 0.1, "%.2f");
            STEP_F("C11", C11, 0.1, "%.2f");
            STEP_F("C12", C12, 0.1, "%.2f");
            STEP_F("C13", C13, 0.1, "%.2f");
            STEP_F("C14", C14, 0.1, "%.2f");
            STEP_F("C15", C15, 0.1, "%.2f");
            STEP_F("C16", C16, 0.1, "%.2f");
            STEP_F("C17", C17, 0.1, "%.2f");
            STEP_F("C18", C18, 0.001, "%.4f");
            STEP_F("C19", C19, 0.1, "%.2f");
            STEP_F("C20", C20, 0.1, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Operational References"))
        {
            STEP_F("Fz0", Fz0, 100.0, "%.0f");
            STEP_F("R0", R0, 0.01, "%.3f");
            STEP_F("Vref", Vref, 1.0, "%.1f");
            STEP_F("Lateral Stiffness", LateralStiffness, 10.0, "%.1f");
            STEP_F("Vertical Stiffness", VerticalStiffness, 10.0, "%.1f");
            STEP_F("Rolling Resistance", RollingResistance, 0.001, "%.4f");
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f) | Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0, ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Mx: %.2f Nm | My: %.2f Nm | Mz: %.2f Nm", ctx.current_out.Mx, ctx.current_out.My, ctx.current_out.Mz);
    ImGui::Text("Mux: %.3f | Muy: %.3f", ctx.current_out.Mux, ctx.current_out.Muy);
}

void RenderClassicPacejkaGraphs(const ClassicPacejkaContext& ctx)
{
    if (ImGui::BeginTabBar("ClassicTabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mx (Overturning)"))
        {
            PlotMoment("Overturning Moment (Mx)", "Slip Angle [deg]", "Mx [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.Mx; }, ctx.alpha_deg_f, ctx.current_out.Mx);
            ImGui::Text("Current Mx: %.2f Nm", ctx.current_out.Mx);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("My (Rolling Resistance)"))
        {
            PlotMoment("Rolling Resistance Moment (My)", "Velocity (Vx) [m/s]", "My [Nm]", 0.0f, 50.0f, ctx.state,
                [](TMState& ts, prec x) { ts.Vx = x; }, [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.My; }, ctx.vx_f, ctx.current_out.My);
            ImGui::Text("Current My: %.2f Nm", ctx.current_out.My);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mz (Aligning Torque)"))
        {
            PlotMoment("Aligning Torque (Mz)", "Slip Angle [deg]", "Mz [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return ClassicPacejka(ctx.params, ts, ctx.featureLevel); },
                [](const TMOutput& out) { return out.Mz; }, ctx.alpha_deg_f, ctx.current_out.Mz);
            ImGui::Text("Current Mz: %.2f Nm", ctx.current_out.Mz);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Force Envelope (Classic vs Ellipse)"))
        {
            static std::vector<prec> fx_pts, fy_pts, ell_x, ell_y;
            fx_pts.clear();
            fy_pts.clear();
            ell_x.clear();
            ell_y.clear();
            const int N_kappa = 50, N_alpha = 50;
            prec fx_max{}, fy_max{};
            for (int i = 0; i < N_kappa; i++)
            {
                prec k = -0.3 + 0.6 * i / (N_kappa - 1);
                for (int j = 0; j < N_alpha; j++)
                {
                    prec a_deg = -15.0 + 30.0 * j / (N_alpha - 1);
                    TMState ts = ctx.state;
                    ts.SlipRatio = k;
                    ts.SlipAngle = a_deg / HLP::R2D;
                    TMOutput out = ClassicPacejka(ctx.params, ts, ctx.featureLevel);
                    fx_pts.push_back(out.Fx);
                    fy_pts.push_back(out.Fy);
                    fx_max = std::max(fx_max, std::abs(out.Fx));
                    fy_max = std::max(fy_max, std::abs(out.Fy));
                }
            }
            const int N_ell = 100;
            for (int i = 0; i < N_ell; i++)
            {
                prec theta = 2.0 * std::numbers::pi_v<prec> *i / (N_ell - 1);
                ell_x.push_back(fy_max * std::cos(theta));
                ell_y.push_back(fx_max * std::sin(theta));
            }
            if (ImPlot::BeginPlot("Tire Force Envelope", ImVec2(-1.0f, -1.0f)))
            {
                ImPlot::SetupAxes("Lateral Force Fy [N]", "Longitudinal Force Fx [N]");
                prec pad_x = fy_max * 0.1;
                if (pad_x < 100) pad_x = 100;
                prec pad_y = fx_max * 0.1;
                if (pad_y < 100) pad_y = 100;
                ImPlot::SetupAxesLimits(-fy_max - pad_x, fy_max + pad_x, -fx_max - pad_y, fx_max + pad_y, ImGuiCond_Always);
                ImPlotSpec scatterSpec;
                scatterSpec.Marker = ImPlotMarker_Circle;
                scatterSpec.MarkerSize = 2.0f;
                scatterSpec.MarkerFillColor = ImVec4(0.2f, 0.8f, 0.8f, 0.6f);
                ImPlot::PlotScatter("Classic Grid", fy_pts.data(), fx_pts.data(), fx_pts.size(), scatterSpec);
                ImPlotSpec ellSpec;
                ellSpec.LineWeight = 3.0f;
                ImPlot::PushStyleColor(ImPlotProp_LineColor, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImPlot::PlotLine("Reference Ellipse", ell_x.data(), ell_y.data(), N_ell, ellSpec);
                ImPlot::PopStyleColor();
                ImPlotSpec ptSpec;
                ptSpec.Marker = ImPlotMarker_Circle;
                ptSpec.MarkerSize = 10.0f;
                ptSpec.MarkerFillColor = ImVec4(1.0, 1.0f, 0.0f, 1.0f);
                ptSpec.MarkerLineColor = ImVec4(0, 0, 0, 1);
                ImPlot::PlotScatter("Current", &ctx.current_out.Fy, &ctx.current_out.Fx, 1, ptSpec);
                ImPlot::EndPlot();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void RenderUAConfig(UAContext& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Inclination (Camber) [deg]", &ctx.camber_deg_f, -10.0f, 10.0f, "%.2f");
        ImGui::SliderFloat("Velocity (Vx) [m/s]", &ctx.vx_f, 0.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Radius (R) [m]", &ctx.r_f, 0.1f, 0.5f, "%.2f");

        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
        ctx.state.InclinationAngle = ctx.camber_deg_f / HLP::R2D;
        ctx.state.Vx = ctx.vx_f;
        ctx.state.R = ctx.r_f;
    }
    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("UA Parameters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Stiffnesses"))
        {
            STEP_F("CSLIP", CSLIP, 1000.0, "%.0f");
            STEP_F("CALPHA", CALPHA, 1000.0, "%.0f");
            STEP_F("CGAMMA", CGAMMA, 100.0, "%.0f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Friction & Geometry"))
        {
            STEP_F("UMin", UMin, 0.01, "%.2f");
            STEP_F("UMax", UMax, 0.01, "%.2f");
            STEP_F("Rolling Resistance", RollingResistance, 0.001, "%.4f");
            STEP_F("Vertical Stiffness", VerticalStiffness, 1000.0, "%.0f");
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f) | Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0, ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Mz: %.2f Nm | My: %.2f Nm", ctx.current_out.Mz, ctx.current_out.My);
    ImGui::Text("Mux: %.3f | Muy: %.3f", ctx.current_out.Mux, ctx.current_out.Muy);
}

void RenderUAGraphs(const UAContext& ctx)
{
    if (ImGui::BeginTabBar("UATabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return UATire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return UATire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return UATire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return UATire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mz (Aligning Torque)"))
        {
            PlotMoment("Aligning Torque (Mz)", "Slip Angle [deg]", "Mz [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return UATire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Mz; }, ctx.alpha_deg_f, ctx.current_out.Mz);
            ImGui::Text("Current Mz: %.2f Nm", ctx.current_out.Mz);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("My (Rolling Resistance)"))
        {
            PlotMoment("Rolling Resistance Moment (My)", "Velocity (Vx) [m/s]", "My [Nm]", 0.0f, 50.0f, ctx.state,
                [](TMState& ts, prec x) { ts.Vx = x; }, [&ctx](const TMState& ts) { return UATire(ctx.params, ts); },
                [](const TMOutput& out) { return out.My; }, ctx.vx_f, ctx.current_out.My);
            ImGui::Text("Current My: %.2f Nm", ctx.current_out.My);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Force Envelope (UA vs Ellipse)"))
        {
            static std::vector<prec> fx_pts, fy_pts, ell_x, ell_y;
            fx_pts.clear();
            fy_pts.clear();
            ell_x.clear();
            ell_y.clear();
            const int N_kappa = 50, N_alpha = 50;
            prec fx_max{}, fy_max{};
            for (int i = 0; i < N_kappa; i++)
            {
                prec k = -0.3 + 0.6 * i / (N_kappa - 1);
                for (int j = 0; j < N_alpha; j++)
                {
                    prec a_deg = -15.0 + 30.0 * j / (N_alpha - 1);
                    TMState ts = ctx.state;
                    ts.SlipRatio = k;
                    ts.SlipAngle = a_deg / HLP::R2D;
                    TMOutput out = UATire(ctx.params, ts);
                    fx_pts.push_back(out.Fx);
                    fy_pts.push_back(out.Fy);
                    fx_max = std::max(fx_max, std::abs(out.Fx));
                    fy_max = std::max(fy_max, std::abs(out.Fy));
                }
            }
            const int N_ell = 100;
            for (int i = 0; i < N_ell; i++)
            {
                prec theta = 2.0 * std::numbers::pi_v<prec> *i / (N_ell - 1);
                ell_x.push_back(fy_max * std::cos(theta));
                ell_y.push_back(fx_max * std::sin(theta));
            }
            if (ImPlot::BeginPlot("Tire Force Envelope", ImVec2(-1.0f, -1.0f)))
            {
                ImPlot::SetupAxes("Lateral Force Fy [N]", "Longitudinal Force Fx [N]");
                prec pad_x = fy_max * 0.1;
                if (pad_x < 100) pad_x = 100;
                prec pad_y = fx_max * 0.1;
                if (pad_y < 100) pad_y = 100;
                ImPlot::SetupAxesLimits(-fy_max - pad_x, fy_max + pad_x, -fx_max - pad_y, fx_max + pad_y, ImGuiCond_Always);
                ImPlotSpec scatterSpec;
                scatterSpec.Marker = ImPlotMarker_Circle;
                scatterSpec.MarkerSize = 2.0f;
                scatterSpec.MarkerFillColor = ImVec4(0.2f, 0.8f, 0.8f, 0.6f);
                ImPlot::PlotScatter("UA Grid", fy_pts.data(), fx_pts.data(), fx_pts.size(), scatterSpec);
                ImPlotSpec ellSpec;
                ellSpec.LineWeight = 3.0f;
                ImPlot::PushStyleColor(ImPlotProp_LineColor, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImPlot::PlotLine("Reference Ellipse", ell_x.data(), ell_y.data(), N_ell, ellSpec);
                ImPlot::PopStyleColor();
                ImPlotSpec ptSpec;
                ptSpec.Marker = ImPlotMarker_Circle;
                ptSpec.MarkerSize = 10.0f;
                ptSpec.MarkerFillColor = ImVec4(1.0, 1.0f, 0.0f, 1.0f);
                ptSpec.MarkerLineColor = ImVec4(0, 0, 0, 1);
                ImPlot::PlotScatter("Current", &ctx.current_out.Fy, &ctx.current_out.Fx, 1, ptSpec);
                ImPlot::EndPlot();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void RenderFialaConfig(FialaContext& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Velocity (Vx) [m/s]", &ctx.vx_f, 0.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Radius (R) [m]", &ctx.r_f, 0.1f, 0.5f, "%.2f");

        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
        ctx.state.Vx = ctx.vx_f;
        ctx.state.R = ctx.r_f;
    }
    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("Fiala Parameters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Stiffnesses"))
        {
            STEP_F("CSLIP", CSLIP, 1000.0, "%.0f");
            STEP_F("CALPHA", CALPHA, 1000.0, "%.0f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Friction & Geometry"))
        {
            STEP_F("UMin", UMin, 0.01, "%.2f");
            STEP_F("UMax", UMax, 0.01, "%.2f");
            STEP_F("Width", Width, 0.01, "%.2f");
            STEP_F("Rolling Resistance", RollingResistance, 0.001, "%.4f");
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f) | Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0, ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Mz: %.2f Nm | My: %.2f Nm", ctx.current_out.Mz, ctx.current_out.My);
    ImGui::Text("Mux: %.3f | Muy: %.3f", ctx.current_out.Mux, ctx.current_out.Muy);
}

void RenderFialaGraphs(const FialaContext& ctx)
{
    if (ImGui::BeginTabBar("FialaTabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return ClassicFiala(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return ClassicFiala(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return ClassicFiala(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return ClassicFiala(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mz (Aligning Torque)"))
        {
            PlotMoment("Aligning Torque (Mz)", "Slip Angle [deg]", "Mz [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return ClassicFiala(ctx.params, ts); },
                [](const TMOutput& out) { return out.Mz; }, ctx.alpha_deg_f, ctx.current_out.Mz);
            ImGui::Text("Current Mz: %.2f Nm", ctx.current_out.Mz);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("My (Rolling Resistance)"))
        {
            PlotMoment("Rolling Resistance Moment (My)", "Velocity (Vx) [m/s]", "My [Nm]", 0.0f, 50.0f, ctx.state,
                [](TMState& ts, prec x) { ts.Vx = x; }, [&ctx](const TMState& ts) { return ClassicFiala(ctx.params, ts); },
                [](const TMOutput& out) { return out.My; }, ctx.vx_f, ctx.current_out.My);
            ImGui::Text("Current My: %.2f Nm", ctx.current_out.My);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Force Envelope (Fiala vs Ellipse)"))
        {
            static std::vector<prec> fx_pts, fy_pts, ell_x, ell_y;
            fx_pts.clear();
            fy_pts.clear();
            ell_x.clear();
            ell_y.clear();
            const int N_kappa = 50, N_alpha = 50;
            prec fx_max{}, fy_max{};
            for (int i = 0; i < N_kappa; i++)
            {
                prec k = -0.3 + 0.6 * i / (N_kappa - 1);
                for (int j = 0; j < N_alpha; j++)
                {
                    prec a_deg = -15.0 + 30.0 * j / (N_alpha - 1);
                    TMState ts = ctx.state;
                    ts.SlipRatio = k;
                    ts.SlipAngle = a_deg / HLP::R2D;
                    TMOutput out = ClassicFiala(ctx.params, ts);
                    fx_pts.push_back(out.Fx);
                    fy_pts.push_back(out.Fy);
                    fx_max = std::max(fx_max, std::abs(out.Fx));
                    fy_max = std::max(fy_max, std::abs(out.Fy));
                }
            }
            const int N_ell = 100;
            for (int i = 0; i < N_ell; i++)
            {
                prec theta = 2.0 * std::numbers::pi_v<prec> *i / (N_ell - 1);
                ell_x.push_back(fy_max * std::cos(theta));
                ell_y.push_back(fx_max * std::sin(theta));
            }
            if (ImPlot::BeginPlot("Tire Force Envelope", ImVec2(-1.0f, -1.0f)))
            {
                ImPlot::SetupAxes("Lateral Force Fy [N]", "Longitudinal Force Fx [N]");
                prec pad_x = fy_max * 0.1;
                if (pad_x < 100) pad_x = 100;
                prec pad_y = fx_max * 0.1;
                if (pad_y < 100) pad_y = 100;
                ImPlot::SetupAxesLimits(-fy_max - pad_x, fy_max + pad_x, -fx_max - pad_y, fx_max + pad_y, ImGuiCond_Always);
                ImPlotSpec scatterSpec;
                scatterSpec.Marker = ImPlotMarker_Circle;
                scatterSpec.MarkerSize = 2.0f;
                scatterSpec.MarkerFillColor = ImVec4(0.2f, 0.8f, 0.8f, 0.6f);
                ImPlot::PlotScatter("Fiala Grid", fy_pts.data(), fx_pts.data(), fx_pts.size(), scatterSpec);
                ImPlotSpec ellSpec;
                ellSpec.LineWeight = 3.0f;
                ImPlot::PushStyleColor(ImPlotProp_LineColor, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImPlot::PlotLine("Reference Ellipse", ell_x.data(), ell_y.data(), N_ell, ellSpec);
                ImPlot::PopStyleColor();
                ImPlotSpec ptSpec;
                ptSpec.Marker = ImPlotMarker_Circle;
                ptSpec.MarkerSize = 10.0f;
                ptSpec.MarkerFillColor = ImVec4(1.0, 1.0f, 0.0f, 1.0f);
                ptSpec.MarkerLineColor = ImVec4(0, 0, 0, 1);
                ImPlot::PlotScatter("Current", &ctx.current_out.Fy, &ctx.current_out.Fx, 1, ptSpec);
                ImPlot::EndPlot();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void RenderDugoffConfig(DugoffContext& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");

        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
    }
    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("Dugoff Parameters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Stiffnesses & Friction"))
        {
            STEP_F("CX (Long. Stiffness)", CX, 1000.0, "%.0f");
            STEP_F("CY (Cornering Stiffness)", CY, 1000.0, "%.0f");
            STEP_F("MuF (Friction Coeff)", MuF, 0.01, "%.2f");
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f) | Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0, ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Mux: %.3f | Muy: %.3f", ctx.current_out.Mux, ctx.current_out.Muy);
}

void RenderDugoffGraphs(const DugoffContext& ctx)
{
    if (ImGui::BeginTabBar("DugoffTabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return DugoffTire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return DugoffTire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return DugoffTire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return DugoffTire(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void RenderTire521Config(Tire521Context& ctx)
{
    auto& params = ctx.params;
    if (ImGui::CollapsingHeader("Tire State Inputs", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Vertical Load (Fz) [N]", &ctx.fz_f, 100.0f, 10000.0f, "%.0f");
        ImGui::SliderFloat("Slip Angle [deg]", &ctx.alpha_deg_f, -20.0f, 20.0f, "%.2f");
        ImGui::SliderFloat("Slip Ratio", &ctx.slip_ratio_f, -1.0f, 1.0f, "%.3f");
        ImGui::SliderFloat("Velocity (Vx) [m/s]", &ctx.vx_f, 0.0f, 100.0f, "%.1f");
        ImGui::SliderFloat("Radius (R) [m]", &ctx.r_f, 0.1f, 0.5f, "%.2f");

        ctx.state.Fz = ctx.fz_f;
        ctx.state.SlipAngle = ctx.alpha_deg_f / HLP::R2D;
        ctx.state.SlipRatio = ctx.slip_ratio_f;
        ctx.state.Vx = ctx.vx_f;
        ctx.state.R = ctx.r_f;
    }
    UIConfig::RenderFzDeltaConfig();

    if (ImGui::CollapsingHeader("Tire521 Parameters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Vertical"))
        {
            STEP_F("Vertical Stiffness", VerticalStiffness, 1000.0, "%.0f");
            STEP_F("Vertical Damping", VerticalDamping, 10.0, "%.1f");
            STEP_F("Stiffness Exponent", VerticalStiffnessExponent, 0.01, "%.2f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Lateral / Aligning"))
        {
            STEP_F("Lateral Stiffness", LateralStiffness, 1000.0, "%.0f");
            STEP_F("Cornering Stiffness Coeff", CorneringStiffnessCoefficient, 1.0, "%.1f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Friction"))
        {
            STEP_F("Mu Static", MuStatic, 0.01, "%.2f");
            STEP_F("Mu Dynamic", MuDynamic, 0.01, "%.2f");
            STEP_F("Mu Static Velocity", MuStaticVelocity, 0.1, "%.1f");
            STEP_F("Mu Dynamic Velocity", MuDynamicVelocity, 0.1, "%.1f");
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Rolling Resistance / Geometry"))
        {
            STEP_F("Rolling Resistance Coeff", RollingResistanceCoefficient, 0.001, "%.4f");
            STEP_F("Unloaded Radius", UnloadedRadius, 0.001, "%.3f");
            ImGui::TreePop();
        }
    }
    ImGui::Separator();
    ImGui::Text("Current Outputs:");
    ImGui::Text("Fx: %.2f N (Pure: %.2f) | Fy: %.2f N (Pure: %.2f)", ctx.current_out.Fx, ctx.current_out.Fx0, ctx.current_out.Fy, ctx.current_out.Fy0);
    ImGui::Text("Mx: %.2f Nm | My: %.2f Nm | Mz: %.2f Nm", ctx.current_out.Mx, ctx.current_out.My, ctx.current_out.Mz);
    ImGui::Text("Mux: %.3f | Muy: %.3f", ctx.current_out.Mux, ctx.current_out.Muy);
}

void RenderTire521Graphs(const Tire521Context& ctx)
{
    if (ImGui::BeginTabBar("Tire521Tabs"))
    {
        if (ImGui::BeginTabItem("Fx0 vs Slip Ratio"))
        {
            PlotPureForceFzVariations("Pure Longitudinal Force (Fx0)", "Slip Ratio", "Fx0 [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; ts.SlipAngle = 0; },
                [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; }, ctx.slip_ratio_f, ctx.current_out.Fx0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Fy0 vs Slip Angle"))
        {
            PlotPureForceFzVariations("Pure Lateral Force (Fy0)", "Slip Angle [deg]", "Fy0 [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; ts.SlipRatio = 0; },
                [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; }, ctx.alpha_deg_f, ctx.current_out.Fy0);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Combined vs Pure"))
        {
            float avail_width = ImGui::GetContentRegionAvail().x;
            float half_width = (avail_width - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

            ImGui::BeginChild("FxCombinedPlot", ImVec2(half_width, 0), false);
            PlotCombinedVsPure("Fx: Pure vs Combined", "Slip Ratio", "Fx [N]", -1.0f, 1.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipRatio = x; },
                [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fx0; },
                [](const TMOutput& out) { return out.Fx; });
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("FyCombinedPlot", ImVec2(0, 0), false);
            PlotCombinedVsPure("Fy: Pure vs Combined", "Slip Angle [deg]", "Fy [N]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; },
                [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.Fy0; },
                [](const TMOutput& out) { return out.Fy; });
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mx (Overturning)"))
        {
            PlotMoment("Overturning Moment (Mx)", "Slip Angle [deg]", "Mx [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.Mx; }, ctx.alpha_deg_f, ctx.current_out.Mx);
            ImGui::Text("Current Mx: %.2f Nm", ctx.current_out.Mx);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("My (Rolling Resistance)"))
        {
            PlotMoment("Rolling Resistance Moment (My)", "Velocity (Vx) [m/s]", "My [Nm]", 0.0f, 50.0f, ctx.state,
                [](TMState& ts, prec x) { ts.Vx = x; }, [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.My; }, ctx.vx_f, ctx.current_out.My);
            ImGui::Text("Current My: %.2f Nm", ctx.current_out.My);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Mz (Aligning Torque)"))
        {
            PlotMoment("Aligning Torque (Mz)", "Slip Angle [deg]", "Mz [Nm]", -20.0f, 20.0f, ctx.state,
                [](TMState& ts, prec x) { ts.SlipAngle = x / HLP::R2D; }, [&ctx](const TMState& ts) { return Tire521(ctx.params, ts); },
                [](const TMOutput& out) { return out.Mz; }, ctx.alpha_deg_f, ctx.current_out.Mz);
            ImGui::Text("Current Mz: %.2f Nm", ctx.current_out.Mz);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Force Envelope (Tire521 vs Ellipse)"))
        {
            static std::vector<prec> fx_pts, fy_pts, ell_x, ell_y;
            fx_pts.clear();
            fy_pts.clear();
            ell_x.clear();
            ell_y.clear();
            const int N_kappa = 50, N_alpha = 50;
            prec fx_max{}, fy_max{};
            for (int i = 0; i < N_kappa; i++)
            {
                prec k = -0.3 + 0.6 * i / (N_kappa - 1);
                for (int j = 0; j < N_alpha; j++)
                {
                    prec a_deg = -15.0 + 30.0 * j / (N_alpha - 1);
                    TMState ts = ctx.state;
                    ts.SlipRatio = k;
                    ts.SlipAngle = a_deg / HLP::R2D;
                    TMOutput out = Tire521(ctx.params, ts);
                    fx_pts.push_back(out.Fx);
                    fy_pts.push_back(out.Fy);
                    fx_max = std::max(fx_max, std::abs(out.Fx));
                    fy_max = std::max(fy_max, std::abs(out.Fy));
                }
            }
            const int N_ell = 100;
            for (int i = 0; i < N_ell; i++)
            {
                prec theta = 2.0 * std::numbers::pi_v<prec> *i / (N_ell - 1);
                ell_x.push_back(fy_max * std::cos(theta));
                ell_y.push_back(fx_max * std::sin(theta));
            }
            if (ImPlot::BeginPlot("Tire Force Envelope", ImVec2(-1.0f, -1.0f)))
            {
                ImPlot::SetupAxes("Lateral Force Fy [N]", "Longitudinal Force Fx [N]");
                prec pad_x = fy_max * 0.1;
                if (pad_x < 100) pad_x = 100;
                prec pad_y = fx_max * 0.1;
                if (pad_y < 100) pad_y = 100;
                ImPlot::SetupAxesLimits(-fy_max - pad_x, fy_max + pad_x, -fx_max - pad_y, fx_max + pad_y, ImGuiCond_Always);
                ImPlotSpec scatterSpec;
                scatterSpec.Marker = ImPlotMarker_Circle;
                scatterSpec.MarkerSize = 2.0f;
                scatterSpec.MarkerFillColor = ImVec4(0.2f, 0.8f, 0.8f, 0.6f);
                ImPlot::PlotScatter("Tire521 Grid", fy_pts.data(), fx_pts.data(), fx_pts.size(), scatterSpec);
                ImPlotSpec ellSpec;
                ellSpec.LineWeight = 3.0f;
                ImPlot::PushStyleColor(ImPlotProp_LineColor, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
                ImPlot::PlotLine("Reference Ellipse", ell_x.data(), ell_y.data(), N_ell, ellSpec);
                ImPlot::PopStyleColor();
                ImPlotSpec ptSpec;
                ptSpec.Marker = ImPlotMarker_Circle;
                ptSpec.MarkerSize = 10.0f;
                ptSpec.MarkerFillColor = ImVec4(1.0, 1.0f, 0.0f, 1.0f);
                ptSpec.MarkerLineColor = ImVec4(0, 0, 0, 1);
                ImPlot::PlotScatter("Current", &ctx.current_out.Fy, &ctx.current_out.Fx, 1, ptSpec);
                ImPlot::EndPlot();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void RenderTireCentralUI()
{
    ImGui::Begin("TireCentral - Tire Model Tool", nullptr, ImGuiWindowFlags_NoScrollbar);

    static SimplePacejkaContext simpleCtx{};
    static MFContext mfCtx{};
    static ClassicPacejkaContext classicCtx{};
    static FialaContext fialaCtx{};
    static UAContext uaCtx{};
    static DugoffContext dugoffCtx{};
    static Tire521Context tire521Ctx{};

    if (ImGui::Button("Save Config"))
    {
        const char* filterPatterns[1] = { "*.json" };
        const char* savePath = tinyfd_saveFileDialog("Save Tire Configuration", "tire_config.json", 1, filterPatterns, "JSON Files");
        if (savePath != nullptr)
        {
            SaveTireConfig(savePath, simpleCtx, mfCtx, classicCtx, fialaCtx, uaCtx, dugoffCtx, tire521Ctx);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Config"))
    {
        const char* filterPatterns[1] = { "*.json" };
        const char* loadPath = tinyfd_openFileDialog("Load Tire Configuration", "", 1, filterPatterns, "JSON Files", 0);
        if (loadPath != nullptr)
        {
            LoadTireConfig(loadPath, simpleCtx, mfCtx, classicCtx, fialaCtx, uaCtx, dugoffCtx, tire521Ctx);
        }
    }
    ImGui::Separator();

    static int model_type = 0;
    ImGui::SetNextItemWidth(200.0f);
    ImGui::Combo("Tire Model", &model_type, "Simple Pacejka\0Magic Formula\0Classic Pacejka\0Fiala\0UA\0Dugoff\0Tire 521\0");
    ImGui::Separator();

    float configWidth = ImGui::GetWindowWidth() * 0.35f;
    ImGui::BeginChild("ConfigPanel", ImVec2(configWidth, 0), true, ImGuiWindowFlags_NoScrollbar);

    switch (model_type)
    {
    case 0:
    {
        RenderSimplePacejkaConfig(simpleCtx);
        simpleCtx.current_out = SimplePacejka(simpleCtx.params, simpleCtx.state);
        break;
    }
    case 1:
    {
        RenderMFConfig(mfCtx);
        mfCtx.current_out = MF(mfCtx.params, mfCtx.state, mfCtx.featureLevel, mfCtx.TurnSlip);
        break;
    }
    case 2:
    {
        RenderClassicPacejkaConfig(classicCtx);
        classicCtx.current_out = ClassicPacejka(classicCtx.params, classicCtx.state, classicCtx.featureLevel);
        break;
    }
    case 3:
    {
        RenderFialaConfig(fialaCtx);
        fialaCtx.current_out = ClassicFiala(fialaCtx.params, fialaCtx.state);
        break;
    }
    case 4:
    {
        RenderUAConfig(uaCtx);
        uaCtx.current_out = UATire(uaCtx.params, uaCtx.state);
        break;
    }
    case 5:
    {
        RenderDugoffConfig(dugoffCtx);
        dugoffCtx.current_out = DugoffTire(dugoffCtx.params, dugoffCtx.state);
        break;
    }
    case 6:
    {
        RenderTire521Config(tire521Ctx);
        tire521Ctx.current_out = Tire521(tire521Ctx.params, tire521Ctx.state);
        break;
    }
    }

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("GraphPanel", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);

    switch (model_type)
    {
    case 0:
    {
        RenderSimplePacejkaGraphs(simpleCtx);
        break;
    }
    case 1:
    {
        RenderMFGraphs(mfCtx);
        break;
    }
    case 2:
    {
        RenderClassicPacejkaGraphs(classicCtx);
        break;
    }
    case 3:
    {
        RenderFialaGraphs(fialaCtx);
        break;
    }
    case 4:
    {
        RenderUAGraphs(uaCtx);
        break;
    }
    case 5:
    {
        RenderDugoffGraphs(dugoffCtx);
        break;
    }
    case 6:
    {
        RenderTire521Graphs(tire521Ctx);
        break;
    }
    }

    ImGui::EndChild();
    ImGui::End();
}

int main(int, char**)
{
    glfwSetErrorCallback([](int error, const char* description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        });
    if (!glfwInit()) return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "TireCentral", nullptr, nullptr);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderTireCentralUI();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.05f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}