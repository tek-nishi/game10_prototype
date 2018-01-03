//
// 一人ゲームジャム
// プロトタイプ
//

#include "lib/framework.hpp"
#include "panel.hpp"
#include "field.hpp"


enum Size {
  WIDTH  = 800,
  HEIGHT = 600
};



// パネル１枚表示
void drawPanel(int number, glm::ivec2 pos, u_int rotation, const Texture& image) {
  int tx = (number % 8) * 128;
  int ty = (number / 8) * 128;

  const float r_tbl[] = {
    0.0f,
    M_PI * 0.5f,
    M_PI,
    M_PI * 1.5f 
  };

  drawTextureBox(pos.x, pos.y,
                 128, 128,
                 tx, ty,
                 128, 128,
                 image, Color(1, 1, 1),
                 r_tbl[rotation], Vec2f(1, 1),
                 Vec2f(64, 64));
}

// Fieldのパネルをすべて表示
void drawFieldPanels(const std::vector<PanelStatus>& panels, const Texture& image) {
  for (const auto& p : panels) {
    drawPanel(p.number, p.position * 128, p.rotation, image);
  }
}

// Fieldの置ける場所をすべて表示
void drawFieldBlank(const std::vector<glm::ivec2>& blank) {
  for (const auto& p : blank) {
    drawFillBox(p.x * 128 - 64, p.y * 128 - 64,
                128, 128,
                Color(0.3, 0.3, 0.3));
  }
}



// Grid表示
void drawFieldGrid() {
  for (int x = 0; x < 8; ++x) {
    drawLine(x * 128 - 512 - 64, -512,
             x * 128 - 512 - 64,  512,
             1,
             Color(0.5, 0.5, 0.5));
  }

  for (int y = 0; y < 8; ++y) {
    drawLine(-512, y * 128 - 512 - 64,
              512, y * 128 - 512 - 64,
             1,
             Color(0.5, 0.5, 0.5));
  }
}


// xをyで丸めこむ
int roundValue(int x, int y) {
  return (x > 0) ? (x + y / 2) / y
                 : (x - y / 2) / y;
}


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
    can_put = (edge[i] == field_panel_edge[(i + 2) % 4]); 
  }

  return can_put;
}


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
