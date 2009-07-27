/*
 *  Copyright 2009 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
/* 
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

namespace('widget');

///////////////////////////////////////////////////////////////
// class Sample

widget.Sample = Class.create();
widget.Sample.prototype.extend(webapp.widget.Widget.prototype).extend
({
  Create: function(tParent)
  {
    var aControls = [ 'Label', 'Edit', 'LabeledCheckbox', 'Select' ];
    
    this.tCtrls = {};
    
    this.tTable = new webapp.ui.Table(tParent);
    for(var nIndex = 0; nIndex != aControls.length; ++nIndex)
    {
      var sControl = aControls[nIndex];
      var tCol = this.tTable.AddRowCell();
      this.tCtrls['t' + sControl] = new webapp.ui[sControl](tCol);
    }
    
    this.tCtrls.tLabel.SetCaption(_('Sample Label'));
    this.tCtrls.tLabeledCheckbox.SetCaption(_('Sample LabeledCheckbox'));
    this.tCtrls.tSelect.SetItems
    ([
      [1, "item1"],
      [2, "item2"],
      [3, "item3"]
    ]);
    
    return this.tWidget;
  },
  
  DeserializeData: function()
  {
    for(var tIndex in this.tCtrls)
    {
      var tItem = this.tCtrls[tIndex];
      if (tItem.SetValue)
      {
        tItem.SetValue(this.tProperties[tIndex]);
      }
    }
  },
  
  SerializeData: function()
  {
    for(var tIndex in this.tCtrls)
    {
      var tItem = this.tCtrls[tIndex];
      if (tItem.GetValue)
      {
        this.tProperties[tIndex] = tItem.GetValue();
      }
    }
  },
  
  GetModifyData: function()
  {
    return true;
  }
});
