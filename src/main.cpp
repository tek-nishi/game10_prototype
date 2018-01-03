//
// 一人ゲームジャム
// プロトタイプ
//

#include "lib/framework.hpp"
#include <iostream>
#include "utility.hpp"
#include "logic.hpp"
#include "view.hpp"


enum Size {
  WIDTH  = 800,
  HEIGHT = 600
};


int main() {
  AppEnv env(Size::WIDTH, Size::HEIGHT);
  
  // パネル情報
  auto panels = createPanels();

  Texture panel_image("res/panels.png");

  // 完成したのを表示
  bool disp_completed = false;
  // 制限時間有効
  bool active_timeup = true;

  while (1) {
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

    // 手持ちのパネル
    int hand_panel = waiting_panels[0];
    u_int hand_rotation = 0;

    // 完成した森
    std::vector<std::vector<glm::ivec2>> completed_forests;
    // 完成した道
    std::vector<std::vector<glm::ivec2>> completed_path;
    
    // 1分モード
    int game_time = 60 * 60 * 1;

    while (1) {
      if (!env.isOpen()) exit(0);

      env.begin();

      if (env.isKeyPushed('D')) {
        disp_completed = !disp_completed;
      }
      if (env.isKeyPushed('T')) {
        active_timeup = !active_timeup;
      }
      if (env.isKeyPushed('R')) {
        break;
      }

      // 時間切れ判定
      if (active_timeup && !--game_time) {
        break;
      }

      // あらかじめマウスカーソル位置→Field位置だけ計算
      auto pos = env.mousePosition();
      glm::ivec2 mouse_pos(pos.x(), pos.y());
      glm::ivec2 field_pos(roundValue(mouse_pos.x, PANEL_SIZE), roundValue(mouse_pos.y, PANEL_SIZE));

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
            auto completed = isCompleteAttribute(Panel::FOREST, field_pos, field, panels);
            if (!completed.empty()) {
              // 得点
              DOUT << "Forest: " << completed.size() << '\n';
              for (const auto& comp : completed) {
                DOUT << " Point: " << comp.size() << '\n';
              }
              DOUT << std::endl;

              // TIPS コンテナ同士の連結
              std::copy(std::begin(completed), std::end(completed), std::back_inserter(completed_forests));
            }
          }
          {
            // 道完成チェック
            auto completed = isCompleteAttribute(Panel::PATH, field_pos, field, panels);
            if (!completed.empty()) {
              // 得点
              DOUT << "  Path: " << completed.size() << '\n';
              for (const auto& comp : completed) {
                DOUT << " Point: " << comp.size() << '\n';
              }
              DOUT << std::endl;

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

      if (disp_completed) {
        // 完成ずみ森の表示
        for (const auto& comp : completed_forests) {
          for (const auto& pos : comp) {
            drawFillBox(pos.x * PANEL_SIZE - PANEL_SIZE / 2, pos.y * PANEL_SIZE - PANEL_SIZE / 2,
                        PANEL_SIZE, PANEL_SIZE,
                        Color(0, 0.5, 0.0, 0.5));
          }
        }
        // 完成ずみの道の表示
        for (const auto& comp : completed_path) {
          for (const auto& pos : comp) {
            drawFillBox(pos.x * PANEL_SIZE - PANEL_SIZE / 2, pos.y * PANEL_SIZE - PANEL_SIZE / 2,
                        PANEL_SIZE, PANEL_SIZE,
                        Color(0, 0.0, 0.5, 0.5));
          }
        }
      }

      {
        // 手持ちのパネル
        // 左クリック時に収まりそう
        if (can_put) {
          drawFillBox(field_pos.x * PANEL_SIZE - PANEL_SIZE / 2, field_pos.y * PANEL_SIZE - PANEL_SIZE / 2,
                      PANEL_SIZE, PANEL_SIZE,
                      Color(0.5, 0, 0));
        }

        drawPanel(hand_panel, mouse_pos, hand_rotation, panel_image);
      }

      env.end();
    }
    env.flushInput();

    // 結果
    DOUT << "Forest: " << completed_forests.size() << '\n'
         << "  area: " << countTotalAttribute(completed_forests, field, panels) << '\n'
         << "  Path: " << completed_path.size() << '\n'
         << "length: " << countTotalAttribute(completed_path, field, panels) << '\n'
         << "  Town: " << countTown(completed_path, field, panels)
         << std::endl;

    auto field_panels = field.enumeratePanels();

    // 結果
    while (1) {
      if (!env.isOpen()) exit(0);
    
      env.begin();

      if (env.isButtonPushed(Mouse::LEFT)) {
        break;
      }
      

      drawFieldGrid();
      drawFieldPanels(field_panels, panel_image);

      env.end();
    }
    env.flushInput();
  }
}
