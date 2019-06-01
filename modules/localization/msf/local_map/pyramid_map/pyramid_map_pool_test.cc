/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include <gtest/gtest.h>
#include <boost/filesystem.hpp>

#include "modules/localization/msf/local_map/pyramid_map/pyramid_map.h"
#include "modules/localization/msf/local_map/pyramid_map/pyramid_map_config.h"
#include "modules/localization/msf/local_map/pyramid_map/pyramid_map_pool.h"

#include "modules/localization/msf/local_map/base_map/base_map.h"
#include "modules/localization/msf/local_map/base_map/base_map_config.h"
#include "modules/localization/msf/local_map/base_map/base_map_pool.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

namespace apollo {
namespace localization {
namespace msf {

class PyramidMapPoolTestSuite : public ::testing::Test {
 protected:
  PyramidMapPoolTestSuite() {}
  virtual ~PyramidMapPoolTestSuite() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
};

TEST_F(PyramidMapPoolTestSuite, pyramid_map_pool_fixed) {
  // init config
  PyramidMapConfig* config = new PyramidMapConfig("lossy_full_alt");
  config->SetMapNodeSize(2, 2);
  config->resolution_num_ = 1;
  config->map_folder_path_ = "test_map_pool";

  PyramidMapNodePool* pool = new PyramidMapNodePool(2, 3);
  pool->Initial(config);
  EXPECT_EQ(pool->GetPoolSize(), 2);

  // alloc map node
  BaseMapNode* node1 = pool->AllocMapNode();
  BaseMapNode* node2 = pool->AllocMapNode();
  BaseMapNode* node3 = pool->AllocMapNode();
  EXPECT_TRUE(node3 == NULL);

  // free node
  pool->FreeMapNode(node2);
  pool->FreeMapNode(node1);

  // Release
  pool->Release();
  EXPECT_EQ(pool->GetPoolSize(), 0);

  if (pool != NULL) {
    delete pool;
    pool = NULL;
  }
  if (config != NULL) {
    delete config;
    config = NULL;
  }
}

TEST_F(PyramidMapPoolTestSuite, pyramid_map_pool_not_fixed) {
  // init config
  PyramidMapConfig* config = new PyramidMapConfig("lossy_full_alt");
  config->SetMapNodeSize(2, 2);
  config->resolution_num_ = 1;
  config->map_folder_path_ = "test_map_pool";

  PyramidMapNodePool* pool = new PyramidMapNodePool(2, 3);
  pool->Initial(config, false);
  EXPECT_EQ(pool->GetPoolSize(), 2);

  // alloc map node
  BaseMapNode* node1 = pool->AllocMapNode();
  BaseMapNode* node2 = pool->AllocMapNode();
  BaseMapNode* node3 = pool->AllocMapNode();
  EXPECT_TRUE(node3 != NULL);
  EXPECT_EQ(pool->GetPoolSize(), 3);

  pool->FreeMapNode(node2);
  pool->FreeMapNode(node1);

  // Release
  pool->Release();
  EXPECT_EQ(pool->GetPoolSize(), 0);

  if (pool != NULL) {
    delete pool;
    pool = NULL;
  }
  if (config != NULL) {
    delete config;
    config = NULL;
  }
}

}  // namespace msf
}  // namespace localization
}  // namespace apollo