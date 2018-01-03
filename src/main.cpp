//
// 一人ゲームジャム
// プロトタイプ
//

#include "lib/framework.hpp"
#include "utility.hpp"
#include "logic.hpp"
#include "view.hpp"


enum Size {
  WIDTH  = 800,
  HEIGHT = 600
};


int main() {
  AppEnv env(Size::WIDTH, Size::HEIGHT);

  // パネルを用意(通し番号で管理)
  std::vector<int> waiting_panels;
  for (int i = 0; i < 64; ++i) {
    waiting_panels.push_back(i);
  }

  {
    // 最初に置くパネルを取り除いてからシャッフル
    auto it = std::find(std::begin(waiting_panels), std::end(waiting_panels), 42);
    waiting_panels.erase(it);

    std::mt19937 engine;
    std::shuffle(std::begin(waiting_panels), std::end(waiting_panels), engine);
  }
  

  // 場を用意
  Field field;
  // 最初のパネルを設置
  field.addPanel(42, {0, 0}, 0);

  // パネル情報
  auto panels = createPanels();

  // 手持ちのパネル
  int hand_panel = waiting_panels[0];
  u_int hand_rotation = 0;

  // 完成した森
  std::vector<std::vector<glm::ivec2>> completed_forests;
  // 完成した道
  std::vector<std::vector<glm::ivec2>> completed_path;
  

  Texture panel_image("res/panels.png");


  while (env.isOpen()) {
    env.begin();

    // あらかじめマウスカーソル位置→Field位置だけ計算
    auto pos = env.mousePosition();
    glm::ivec2 mouse_pos(pos.x(), pos.y());
    glm::ivec2 field_pos(roundValue(mouse_pos.x, 128), roundValue(mouse_pos.y, 128));

    auto field_panels = field.enumeratePanels();
    auto blank = field.searchBlank();

    bool can_put = false;
    {
      if (std::find(std::begin(blank), std::end(blank), field_pos) != std::end(blank)) {
        can_put = canPutPanel(panels[hand_panel], field_pos, hand_rotation,
                              field, panels);
      }

      // 手持ちパネルの操作
      if (env.isButtonPushed(Mouse::LEFT) && can_put) {
        field.addPanel(hand_panel, field_pos, hand_rotation);

        {
          // 森完成チェック
          auto completed = checkForestEdge(field_pos, field, panels);
          if (!completed.empty()) {
            // TIPS コンテナ同士の連結
            std::copy(std::begin(completed), std::end(completed), std::back_inserter(completed_forests));
          }
        }
        {
          // 道完成チェック
          auto completed = checkPathEdge(field_pos, field, panels);
          if (!completed.empty()) {
            // TIPS コンテナ同士の連結
            std::copy(std::begin(completed), std::end(completed), std::back_inserter(completed_path));
          }
        }

        waiting_panels.erase(std::begin(waiting_panels));

        hand_panel = waiting_panels[0];
        hand_rotation = 0;
      }
      else if (env.isButtonPushed(Mouse::RIGHT)) {
        // 適当に回転
        hand_rotation = (hand_rotation + 1) % 4;
      }
    }

    drawFieldGrid();

    drawFieldPanels(field_panels, panel_image);
    drawFieldBlank(blank);

    {
      // 完成ずみ森の表示
      for (const auto& comp : completed_forests) {
        for (const auto& pos : comp) {
          drawFillBox(pos.x * 128 - 64, pos.y * 128 - 64,
                      128, 128,
                      Color(0, 0.5, 0.0, 0.5));
        }
      }
    }
    {
      // 完成ずみの道の表示
      for (const auto& comp : completed_path) {
        for (const auto& pos : comp) {
          drawFillBox(pos.x * 128 - 64, pos.y * 128 - 64,
                      128, 128,
                      Color(0, 0.0, 0.5, 0.5));
        }
      }
    }

    {
      // 手持ちのパネル
      // 左クリック時に収まりそう
      if (can_put) {
        drawFillBox(field_pos.x * 128 - 64, field_pos.y * 128 - 64, 128, 128, Color(0.5, 0, 0));
      }

      drawPanel(hand_panel, mouse_pos, hand_rotation, panel_image);
    }

    env.end();
  }
}
