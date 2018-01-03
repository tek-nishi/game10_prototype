﻿#pragma once

//
// 地形パネル
//

#include <vector>


struct Panel {
  enum {
    // ４辺の構造
    PATH   = 1 << 0,         // 道
    GRASS  = 1 << 1,         // 草原
    FOREST = 1 << 2,         // 森

    EDGE = 1 << 16  // 端
  };

  Panel(int up, int right, int bottom, int left)
    : edge_(4)
  {
    edge_[0] = up;
    edge_[1] = right;
    edge_[2] = bottom;
    edge_[3] = left;
  }

  const std::vector<int>& getEdge() const { return edge_; }

  // 回転ずみの端情報
  std::vector<int> getRotatedEdge(u_int rotation) const {
    std::vector<int> edge = edge_;

    // 左方向へのシフト
    std::rotate(std::begin(edge), std::begin(edge) + rotation, std::end(edge));
    return edge;
  }


private:
  std::vector<int> edge_;     // ４辺の構造



};



// 初期パネル生成
std::vector<Panel> createPanels() {
  std::vector<Panel> panels = {
    { Panel::GRASS,  Panel::FOREST, Panel::FOREST, Panel::FOREST },
    { Panel::PATH,   Panel::PATH,   Panel::FOREST, Panel::FOREST },
    { Panel::FOREST | Panel::EDGE, Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE },
    { Panel::GRASS,  Panel::GRASS,  Panel::PATH,   Panel::PATH },
    
    { Panel::GRASS,  Panel::FOREST, Panel::FOREST, Panel::FOREST },
    { Panel::PATH,   Panel::PATH,   Panel::FOREST, Panel::FOREST },
    { Panel::FOREST | Panel::EDGE, Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE },
    { Panel::GRASS,  Panel::GRASS,  Panel::PATH,   Panel::PATH },
    
    { Panel::FOREST | Panel::EDGE, Panel::GRASS,  Panel::GRASS, Panel::GRASS },
    { Panel::FOREST, Panel::FOREST, Panel::GRASS, Panel::GRASS },
    { Panel::PATH,   Panel::PATH,   Panel::GRASS, Panel::FOREST | Panel::EDGE },
    { Panel::PATH,   Panel::GRASS,  Panel::GRASS, Panel::PATH },
    
    { Panel::FOREST | Panel::EDGE, Panel::GRASS,  Panel::GRASS, Panel::GRASS },
    { Panel::FOREST, Panel::FOREST, Panel::GRASS, Panel::GRASS },
    { Panel::PATH,   Panel::PATH,   Panel::GRASS, Panel::FOREST | Panel::EDGE },
    { Panel::PATH,   Panel::GRASS,  Panel::GRASS, Panel::PATH },
    
    { Panel::GRASS, Panel::FOREST | Panel::EDGE, Panel::GRASS, Panel::FOREST | Panel::EDGE },
    { Panel::GRASS, Panel::PATH,   Panel::PATH,  Panel::FOREST | Panel::EDGE },
    { Panel::GRASS, Panel::PATH,   Panel::GRASS, Panel::PATH },
    { Panel::GRASS, Panel::GRASS,  Panel::PATH,  Panel::PATH },
    
    { Panel::GRASS, Panel::FOREST | Panel::EDGE, Panel::GRASS, Panel::FOREST | Panel::EDGE },
    { Panel::GRASS, Panel::PATH,   Panel::PATH,  Panel::FOREST | Panel::EDGE },
    { Panel::GRASS, Panel::PATH,   Panel::GRASS, Panel::PATH },
    { Panel::GRASS, Panel::GRASS,  Panel::PATH,  Panel::PATH },


    { Panel::GRASS,  Panel::PATH,   Panel::GRASS,  Panel::PATH },
    { Panel::GRASS,  Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE },
    { Panel::FOREST, Panel::FOREST, Panel::FOREST, Panel::PATH | Panel::EDGE },
    { Panel::GRASS,  Panel::FOREST, Panel::GRASS,  Panel::FOREST },
    
    { Panel::GRASS,  Panel::PATH,   Panel::GRASS,  Panel::PATH },
    { Panel::GRASS,  Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE },
    { Panel::FOREST, Panel::FOREST, Panel::FOREST, Panel::PATH | Panel::EDGE },
    { Panel::GRASS,  Panel::FOREST, Panel::GRASS,  Panel::FOREST },
    
    { Panel::GRASS,  Panel::GRASS,  Panel::GRASS, Panel::GRASS },
    { Panel::PATH,   Panel::FOREST | Panel::EDGE, Panel::PATH,  Panel::GRASS },
    { Panel::FOREST, Panel::GRASS,  Panel::GRASS, Panel::FOREST },
    { Panel::GRASS,  Panel::GRASS,  Panel::GRASS, Panel::GRASS },
    
    { Panel::GRASS,  Panel::GRASS,  Panel::GRASS, Panel::GRASS },
    { Panel::PATH,   Panel::FOREST | Panel::EDGE, Panel::PATH,  Panel::GRASS },
    { Panel::FOREST, Panel::GRASS,  Panel::GRASS, Panel::FOREST },
    { Panel::GRASS,  Panel::GRASS,  Panel::GRASS, Panel::GRASS },
    
    { Panel::GRASS, Panel::PATH,   Panel::GRASS,  Panel::PATH },
    { Panel::PATH | Panel::EDGE,  Panel::PATH | Panel::EDGE,   Panel::GRASS,  Panel::PATH | Panel::EDGE },
    { Panel::GRASS, Panel::GRASS,  Panel::GRASS,  Panel::PATH | Panel::EDGE },
    { Panel::GRASS, Panel::FOREST | Panel::EDGE, Panel::FOREST | Panel::EDGE, Panel::GRASS },

    { Panel::GRASS, Panel::PATH,   Panel::GRASS,  Panel::PATH },
    { Panel::PATH | Panel::EDGE,  Panel::PATH | Panel::EDGE,   Panel::GRASS,  Panel::PATH | Panel::EDGE },
    { Panel::GRASS, Panel::GRASS,  Panel::GRASS,  Panel::PATH | Panel::EDGE },
    { Panel::GRASS, Panel::FOREST | Panel::EDGE, Panel::FOREST | Panel::EDGE, Panel::GRASS },
    
    
    { Panel::PATH,   Panel::FOREST, Panel::FOREST, Panel::PATH },
    { Panel::FOREST, Panel::GRASS,  Panel::FOREST, Panel::FOREST },
    { Panel::GRASS,  Panel::GRASS,  Panel::FOREST | Panel::EDGE, Panel::GRASS },
    { Panel::PATH,   Panel::FOREST | Panel::EDGE, Panel::PATH,   Panel::GRASS },
    
    { Panel::PATH,   Panel::FOREST, Panel::FOREST, Panel::PATH },
    { Panel::FOREST, Panel::GRASS,  Panel::FOREST, Panel::FOREST },
    { Panel::GRASS,  Panel::GRASS,  Panel::FOREST | Panel::EDGE, Panel::GRASS },
    { Panel::PATH,   Panel::FOREST | Panel::EDGE, Panel::PATH,   Panel::GRASS },
    
    { Panel::FOREST, Panel::FOREST, Panel::PATH,   Panel::PATH },
    { Panel::FOREST, Panel::FOREST, Panel::FOREST, Panel::FOREST },
    { Panel::FOREST, Panel::PATH | Panel::EDGE,   Panel::FOREST, Panel::FOREST },
    { Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE },
    
    { Panel::FOREST, Panel::FOREST, Panel::PATH,   Panel::PATH },
    { Panel::FOREST, Panel::FOREST, Panel::FOREST, Panel::FOREST },
    { Panel::FOREST, Panel::PATH | Panel::EDGE,   Panel::FOREST, Panel::FOREST },
    { Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE,   Panel::PATH | Panel::EDGE },
  };

  return panels;
}
