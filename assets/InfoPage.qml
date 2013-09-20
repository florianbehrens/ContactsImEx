import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Contacts ImEx")
    }
    
    ScrollView {
	    Container {
	        topPadding: 30
	        bottomPadding: 30
	        leftPadding: 20
	        rightPadding: 20

			ImageView {
	            imageSource: "asset:///images/icon_big.png"
	            horizontalAlignment: HorizontalAlignment.Center
	            scalingMethod: ScalingMethod.AspectFit
	        }		
	    	Label {
				multiline: true
	            textFormat: TextFormat.Html
				text: "<b>ContactsImEx Version 1.0.0</b><br/>" +
				      "© 2013 by Florian Behrens."
			}
	        Label {
	            multiline: true
	            textFormat: TextFormat.Html
	            text: "This application contains parts of the <a href=\"http://www.boost.org\">Boost C++ libraries</a>."
	        }
	        Label {
	            multiline: true
	            textFormat: TextFormat.Html
	            text: "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
                textStyle.textAlign: TextAlign.Justify
	        }
	    }
    }
}
