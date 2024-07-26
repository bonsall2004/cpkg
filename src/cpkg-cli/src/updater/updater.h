/*
 * cpkg
 * Author: bonsall2004
 * Description: 
 */
#pragma once

namespace cpkg::updater
{
  /**
   * @Brief Checks the github repo releases to see if the current installed version is previous to the latest on the git releases
   * @return True if an update is available
   */
  bool check_for_updates();
}