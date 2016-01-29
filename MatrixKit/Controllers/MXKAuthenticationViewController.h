/*
 Copyright 2015 OpenMarket Ltd
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#import <UIKit/UIKit.h>

#import "MXKViewController.h"

#import "MXKAuthInputsView.h"
#import "MXKAuthenticationFallbackWebView.h"

extern NSString *const MXKAuthErrorDomain;

@class MXKAuthenticationViewController;

/**
 `MXKAuthenticationViewController` delegate.
 */
@protocol MXKAuthenticationViewControllerDelegate <NSObject>

/**
 Tells the delegate the authentication process succeeded to add a new account.
 
 @param authenticationViewController the `MXKAuthenticationViewController` instance.
 @param userId the user id of the new added account.
 */
- (void)authenticationViewController:(MXKAuthenticationViewController *)authenticationViewController didLogWithUserId:(NSString*)userId;

@end

/**
 This view controller should be used to manage registration or login flows with matrix home server.
 
 Only the flow based on password is presently supported. Other flows should be added later.
 
 You may add a delegate to be notified when a new account has been added successfully.
 */
@interface MXKAuthenticationViewController : MXKViewController <UITextFieldDelegate, MXKAuthInputsViewDelegate>
{
@protected
    /**
     Array of flows supported by the home server and implemented by the view controller (for the current auth type).
     */
    NSMutableArray *supportedFlows;
    
    /**
     The current selected login flow
     */
    MXLoginFlow *selectedFlow;
}

@property (weak, nonatomic) IBOutlet UIImageView *welcomeImageView;

@property (strong, nonatomic) IBOutlet UIScrollView *authenticationScrollView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *authScrollViewBottomConstraint;

@property (weak, nonatomic) IBOutlet UIView *contentView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *contentViewHeightConstraint;

@property (weak, nonatomic) IBOutlet UILabel *subTitleLabel;

@property (weak, nonatomic) IBOutlet UIView *authInputsContainerView;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *authInputContainerViewHeightConstraint;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *authInputContainerViewMinHeightConstraint;

@property (weak, nonatomic) IBOutlet UILabel *homeServerLabel;
@property (weak, nonatomic) IBOutlet UITextField *homeServerTextField;
@property (weak, nonatomic) IBOutlet UILabel *homeServerInfoLabel;
@property (weak, nonatomic) IBOutlet UILabel *identityServerLabel;
@property (weak, nonatomic) IBOutlet UITextField *identityServerTextField;
@property (weak, nonatomic) IBOutlet UILabel *identityServerInfoLabel;

@property (weak, nonatomic) IBOutlet UIButton *submitButton;
@property (weak, nonatomic) IBOutlet UIButton *authSwitchButton;

@property (strong, nonatomic) IBOutlet UIActivityIndicatorView *authenticationActivityIndicator;
@property (weak, nonatomic) IBOutlet UILabel *noFlowLabel;
@property (weak, nonatomic) IBOutlet UIButton *retryButton;

@property (weak, nonatomic) IBOutlet UIView *authFallbackContentView;
@property (weak, nonatomic) IBOutlet MXKAuthenticationFallbackWebView *authFallbackWebView;
@property (weak, nonatomic) IBOutlet UIButton *cancelAuthFallbackButton;

/**
 The current authentication type
 */
@property (nonatomic) MXKAuthenticationType authType;

/**
 The current selected login flow
 */
@property (nonatomic) MXLoginFlow *selectedFlow;

/**
 The default home server url (nil by default).
 */
@property (nonatomic) NSString *defaultHomeServerUrl;

/**
 The default identity server url (nil by default).
 */
@property (nonatomic) NSString *defaultIdentityServerUrl;

/**
 The delegate for the view controller.
 */
@property (nonatomic) id<MXKAuthenticationViewControllerDelegate> delegate;

/**
 *  Returns the `UINib` object initialized for a `MXKAuthenticationViewController`.
 *
 *  @return The initialized `UINib` object or `nil` if there were errors during initialization
 *  or the nib file could not be located.
 *
 *  @discussion You may override this method to provide a customized nib. If you do,
 *  you should also override `authenticationViewController` to return your
 *  view controller loaded from your custom nib.
 */
+ (UINib *)nib;

/**
 *  Creates and returns a new `MXKAuthenticationViewController` object.
 *
 *  @discussion This is the designated initializer for programmatic instantiation.
 *
 *  @return An initialized `MXKAuthenticationViewController` object if successful, `nil` otherwise.
 */
+ (instancetype)authenticationViewController;

/**
 Register the MXKAuthInputsView class that will be used to display inputs for the designated flow and authentication type.
 
 By default only 'MXKAuthInputsPasswordBasedView' class is registered for 'kMXLoginFlowTypePassword' flow and 'MXKAuthenticationTypeLogin' authentication.
 
 @param authInputsViewClass a MXKAuthInputsView-inherited class that will be used for the designated flow.
 @param flowType the concerned flow type.
 @param authType the concerned authentication type
 */
- (void)registerAuthInputsViewClass:(Class)authInputsViewClass forFlowType:(MXLoginFlowType)flowType andAuthType:(MXKAuthenticationType)authType;

/**
 Check login mechanism supported by the server and the application.
 */
- (void)refreshSupportedAuthFlow;

/**
 Handle supported flows returned by the server.
 */
- (void)handleHomeServerFlows:(NSArray *)flows;

/**
 Customize the MXHTTPClientOnUnrecognizedCertificate block that will be used to handle unrecognized certificate observed during authentication challenge from a server.
 By default we prompt the user by displaying a fingerprint (SHA256) of the certificate. The user is then able to trust or not the certificate.
 
 @param onUnrecognizedCertificateBlock
 */
- (void)setOnUnrecognizedCertificateBlock:(MXHTTPClientOnUnrecognizedCertificate)onUnrecognizedCertificateBlock;

/**
 Action registered on the following events:
 - 'UIControlEventTouchUpInside' for each UIButton instance.
 - 'UIControlEventValueChanged' for each UISwitch instance.
 */
- (IBAction)onButtonPressed:(id)sender;

@end

