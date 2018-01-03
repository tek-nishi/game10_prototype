#pragma once

//
// ゲームロジック
//

#include "panel.hpp"
#include "field.hpp"


// Fieldにパネルが置けるか判定
bool canPutPanel(const Panel& panel, glm::ivec2 pos, u_int rotation,
                 const Field& field, const std::vector<Panel>& panels) {
  // 時計回りに調べる
  const glm::ivec2 offsets[] = {
    {  0,  1 },
    {  1,  0 },
    {  0, -1 },
    { -1,  0 },
  };

  // ４隅の情報
  auto edge = panel.getRotatedEdge(rotation);

  // ４方向確認
  bool can_put = true;
  for (u_int i = 0; can_put && (i < 4); ++i) {
    glm::ivec2 p = pos + offsets[i];
    if (!field.existsPanel(p)) continue;

    // Field上のパネル情報
    const auto& panel_status = field.getPanelStatus(p);
    const auto& field_panel = panels[panel_status.number];
    auto field_panel_edge = field_panel.getRotatedEdge(panel_status.rotation);

    // エッジが一致していないと置けない
    can_put = ((edge[i] & ~Panel::EDGE) == (field_panel_edge[(i + 2) % 4] & ~Panel::EDGE)); 
  }

  return can_put;
}


// 森の端を調べる
bool checkForestEdge(glm::ivec2 pos, u_int direction,
                     const Field& field, const std::vector<Panel>& panels,
                     std::vector<glm::ivec2>& checked,
                     std::vector<glm::ivec2>& completed) {
  // パネルがない→閉じていない
  if (!field.existsPanel(pos)) return false;

  // パネル情報
  const auto& status = field.getPanelStatus(pos);
  const auto& panel  = panels[status.number];
  const auto edge    = panel.getRotatedEdge(status.rotation);

  // そこが端なら判定完了
  if (edge[(direction + 2) % 4] & Panel::EDGE) {
    completed.push_back(pos);
    return true;
  }

  // 調査ずみ？
  if (std::find(std::begin(checked), std::end(checked), pos) != std::end(checked)) {
    return true;
  }
  checked.push_back(pos);

  // 時計回りに調べる
  const glm::ivec2 offsets[] = {
    {  0,  1 },
    {  1,  0 },
    {  0, -1 },
    { -1,  0 },
  };

  for (u_int i = 0; i < 4; ++i) {
    // 戻らない
    if (i == ((direction + 2) % 4)) continue;
    
    auto p = pos + offsets[i];
    if (edge[i] & Panel::FOREST) {
      // その先が閉じているか調査
      if (!checkForestEdge(p, i, field, panels, checked, completed)) return false;
    }
  }

  completed.push_back(pos);

  return true;
}

std::vector<std::vector<glm::ivec2>> checkForestEdge(glm::ivec2 pos,
                                                     const Field& field, const std::vector<Panel>& panels) {
  std::vector<std::vector<glm::ivec2>> completed;

  // パネル情報
  const auto& status = field.getPanelStatus(pos);
  const auto& panel  = panels[status.number];
  const auto edge    = panel.getRotatedEdge(status.rotation);

  // パネルは森の端か途中かの２択(両方含んだ森は無い)
  bool has_forest = false;
  bool has_edge   = false;
  for (u_int i = 0; i < 4; ++i) {
    if (edge[i] & Panel::FOREST) has_forest = true;
    if (edge[i] & Panel::EDGE)   has_edge   = true;
  }

  // 森無し
  if (!has_forest) return completed;

  // 時計回りに調べる
  const glm::ivec2 offsets[] = {
    {  0,  1 },
    {  1,  0 },
    {  0, -1 },
    { -1,  0 },
  };

  if (has_edge) {
    // 端を含んだ森→端ごとに調査
    for (u_int i = 0; i < 4; ++i) {
      auto p = pos + offsets[i];
      if (edge[i] & Panel::FOREST) {
        std::vector<glm::ivec2> checked;
        std::vector<glm::ivec2> comp;
        // その先が閉じているか調査
        if (checkForestEdge(p, i, field, panels, checked, comp)) {
          comp.push_back(pos);
          completed.push_back(comp);
        }
      }
    }
  }
  else {
    // 含んでいない森→すべてで閉じていないとならない
    std::vector<glm::ivec2> checked;
    std::vector<glm::ivec2> comp;
    for (u_int i = 0; i < 4; ++i) {
      auto p = pos + offsets[i];
      if (edge[i] & Panel::FOREST) {
        // その先が閉じているか調査
        if (!checkForestEdge(p, i, field, panels, checked, comp)) {
          // １つでも閉じていなければ調査完了
          return completed;
        }
      }
    }
    completed.push_back(comp);
  }

  return completed;
}


// 地形が完成したか調べる
//   a. 森の完成
//   b. 道の完成
//   c. 平野の完成
void checkTopography(glm::ivec2 pos, const Field& field, const std::vector<Panel>& panels) {

}
