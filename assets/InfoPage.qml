/*
 * (c) Copyright Florian Behrens 2013.
 *
 * This file is part of ContactsImEx.
 *
 * ContactsImEx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ContactsImEx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ContactsImEx.  If not, see <http://www.gnu.org/licenses/>.
 */

import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("About ContactsImEx")
    }
    
    ScrollView {
	    Container {
	        topPadding: 30
	        bottomPadding: 30
	        leftPadding: 20
	        rightPadding: 20

			ImageView {
                imageSource: "asset:///images/ContactsImEx_400x400.png"
	            horizontalAlignment: HorizontalAlignment.Center
	            scalingMethod: ScalingMethod.AspectFit
	        }
	    	Label {
				multiline: true
	            textFormat: TextFormat.Html
				text: "<b>ContactsImEx Version 1.0.1</b><br/>" +
				      "© 2013 by Florian Behrens."
			}
	        
	        Divider {}

	        Label {
	            multiline: true
	            textFormat: TextFormat.Html
                text: "This application utilizes the Tokenizer of the <a href=\"http://www.boost.org\">Boost C++ libraries</a> " + 
                      "for CSV parsing. CSV files must be encoded in UTF-8 and adhere to the format specified by the " +
                      "<a href=\"http://www.boost.org/doc/libs/1_54_0/libs/tokenizer/escaped_list_separator.htm\">Escaped " + 
                      "List Separator</a> using the default characters for separator (,), quote (\"), and escape (\\). " + 
                      "The first line is considered a header record containing the field names. To find out about the " + 
                      "required CSV header row values export a reference contact first."
            }
            Label {
                multiline: true
                textFormat: TextFormat.Html
                text: "CSV format examples:<br/>" +
                "Field 1,Field 2,Field 3<br/>" +
                "Field 1,\"Field 2, with comma\",Field 3<br/>" + 
                "Field 1,Field 2 with \\\"quotes\\\",Field 3<br/>" +
                "Field 1,Field 2 with \\n new line,Field 3<br/>" +
                "Field 1,Field 2 with embedded \\\\,Field 3"
	        }

            Divider {}

	        Label {
	            multiline: true
	            textFormat: TextFormat.Html
	            text: "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
                textStyle.textAlign: TextAlign.Justify
	        }
	    }
    }
}
