package com.geishatokyo.purchase;

import java.util.List;

import org.json.JSONException;

import android.app.Activity;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.util.Log;

import com.android.billing.util.IabHelper;
import com.android.billing.util.IabResult;
import com.android.billing.util.Inventory;
import com.android.billing.util.Purchase;

public class GoogleBilling extends Activity {

	public static boolean bSupport = false;
	public static IabHelper mHelper;
	private static Activity mActivity = null;
	private static GLSurfaceView mGLSurfaceView = null;

	// debug
	public static final boolean isDebug = true;
    // Debug tag, for logging
    static final String TAG = "GoogleBilling";
	// Messages
	public static int INIT_BILLING_NO = 0;
	public static int INIT_BILLING_YES = 1;
	public static int PURCHASE_SUCCESS = 2;
	public static int PURCHASE_ALREADY_PURCHASED = 3;
	public static int PURCHASE_FAIL = 4;
	public static int PURCHASE_USER_CANCELED =5;
	public static int CONSUME_SUCCESS = 6;
	public static int CONSUME_FAIL = 7;
	public static int RESTORERECEIPT_SUCCESS = 8;
	public static int RESTORERECEIPT_FAIL = 9;

    // Item types
    public static final String ITEM_TYPE_INAPP = "inapp";

	public static void setup(Activity activity, GLSurfaceView vw) {
		GoogleBilling.mActivity = activity;
		GoogleBilling.mGLSurfaceView = vw;
	}

	private static native void nativeactionFinished(long callback_address,
			int result);

	private static native boolean nativepaymentTransaction(String sku, String purchaseData,
			                                               String signature, int state);

	private static void paymentTransaction(final String sku, final String purchaseData,
			final String signature, final int state) {
		mGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				nativepaymentTransaction(sku, purchaseData, signature, state);
			}
		});
	}

	/*
	 * Initialize GoogleBilling.
	 */
	public static boolean init(final long callback_address) {
		if (mActivity == null) {
			if (isDebug) {
				Log.d(TAG, "Activity not set!");
			}
			nativeactionFinished(callback_address, INIT_BILLING_NO);
			return false;
		}
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mHelper = new IabHelper(mActivity);
				// Remove before release
				mHelper.enableDebugLogging(true);

				mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
					public void onIabSetupFinished(final IabResult result) {
						if (isDebug) {
							Log.d(TAG, "Result:" + result);
						}

						int resultMessage = INIT_BILLING_NO;
						if (result.isSuccess()) {
							bSupport = true;
							resultMessage = INIT_BILLING_YES;
							Log.d(TAG, "Setup successful. Querying inventory.");
						} else {
							bSupport = false;
							Log.e(TAG, "Problem setting up in-app billing: " + result);
						}
						nativeactionFinished(callback_address, resultMessage);
					}
				});
			}
		});
		return true;
	}

	public static void consumeOwnItem(final String productId, final long callback_address)
	{
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mHelper.queryInventoryAsync(false, new IabHelper.QueryInventoryFinishedListener()
				{
					public void onQueryInventoryFinished(IabResult result, Inventory inventory)
					{
						Log.d(TAG, "Query inventory finished.");
						if (result.isFailure()) {
							Log.e(TAG, "Failed to query inventory: " + result);
							return;
						}

						Log.d(TAG, "Query inventory was successful.");

						Purchase purchase = inventory.getPurchase(productId);
						if(purchase != null) consumePurchase(purchase, callback_address);
						else nativeactionFinished(callback_address,GoogleBilling.CONSUME_FAIL);
					}
				});
			}
		});
    };

	/*
	 * Purchase an item
	 */
	public static void purchase(final String item, final int amount, final long callback_address) {
		if (isDebug) {
			Log.d(TAG, "Trying to purchase item:" + item);
		}
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				String id = android.provider.Settings.Secure.ANDROID_ID;
				mHelper.launchPurchaseFlow(mActivity, item, 10001,
						new IabHelper.OnIabPurchaseFinishedListener() {
							public void onIabPurchaseFinished(IabResult result,
									Purchase purchase) {
								if (isDebug) {
									Log.d(TAG, "purchase result:" + result);
								}
								int resultMsg = GoogleBilling.PURCHASE_FAIL;
								// Purchase success or item already owned
								if (result.isSuccess()) {
									if (isDebug) {
										Log.d(TAG, "Purchase success");
									}
									//resultMsg = GoogleBilling.PURCHASE_SUCCESS;
									paymentTransaction(purchase.getSku(),
							                           purchase.getOriginalJson(),
                                                       purchase.getSignature(),
                                                       purchase.getPurchaseState());
									return;
								}
								if (result.getResponse() == IabHelper.IABHELPER_USER_CANCELLED) {
									resultMsg = GoogleBilling.PURCHASE_USER_CANCELED;
								}
								if (result.getResponse() == IabHelper.BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED) {
									resultMsg = GoogleBilling.PURCHASE_ALREADY_PURCHASED;
									if (isDebug) {
										Log.d(TAG, "Already purchased");
									}
								}
								nativeactionFinished(callback_address, resultMsg);
							}
						}, id);
			}
		});
	}

	public static void consume(final String purchaseData, final String dataSignature, final long callback_address)
	{
        Purchase purchase = null;
        try {
        	purchase = new Purchase(ITEM_TYPE_INAPP, purchaseData, dataSignature);
        }
        catch (JSONException e) {
            Log.e(TAG, "Failed to parse purchase data.");
            e.printStackTrace();
		}
        consumePurchase(purchase, callback_address);
	}

	/*
	 * Consume SKU (item)
	 */
	public static void consumePurchase(final Purchase purchase) { consumePurchase(purchase, 0); }
	public static void consumePurchase(final Purchase purchase,final long callback_address)
	{
        if (isDebug) {
			Log.d(TAG, "Trying to consume item:" + purchase.getSku());
		}
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mHelper.consumeAsync(purchase,new IabHelper.OnConsumeFinishedListener()
				{
					public void onConsumeFinished(Purchase purchase, IabResult result)
					{
						if (result.isSuccess()) {
							if (isDebug)
							{
								Log.d(TAG,"Consumed item:" + purchase.getSku());
							}
							if(callback_address > 0) {
								nativeactionFinished(callback_address,GoogleBilling.CONSUME_SUCCESS);
							}
							return;
						} else {
							if(callback_address > 0) {
								nativeactionFinished(callback_address,GoogleBilling.CONSUME_FAIL);
							}
						}
					}
				});
			}
		});
	}

	public static void restoreReceipt(final long callback_address) {
		if (isDebug) {
			Log.d(TAG, "Trying to restore receipt");
		}
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mHelper.queryInventoryAsync(false, new IabHelper.QueryInventoryFinishedListener()
				{
					public void onQueryInventoryFinished(IabResult result, Inventory inventory)
					{
						Log.d(TAG, "Query inventory finished.");
						if (result.isFailure()) {
							Log.e(TAG, "Failed to query inventory: " + result);
							return;
						}

						Log.d(TAG, "Query inventory was successful.");

						List<Purchase> purchaseList = inventory.getAllPurchases();
                        if(!purchaseList.isEmpty()) {
                            Purchase purchase = purchaseList.get(0);

                            if(purchase != null) {
                                paymentTransaction(purchase.getSku(),
                                                   purchase.getOriginalJson(),
                                                   purchase.getSignature(),
                                                   purchase.getPurchaseState());
                                nativeactionFinished(callback_address,GoogleBilling.RESTORERECEIPT_SUCCESS);
                            } else {
                                nativeactionFinished(callback_address,GoogleBilling.RESTORERECEIPT_FAIL);
                            }
                        } else {
                            Log.d(TAG, "There is no purchase info in inventory.");
                            nativeactionFinished(callback_address,GoogleBilling.RESTORERECEIPT_FAIL);
                        }
					}
				});
			}
		});
	}

	/*
	 * Stop billing servie
	 */
	public static void stop() {
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {

				if (mHelper != null)
					mHelper.dispose();
				mHelper = null;
			}
		});
	}

	public static boolean handleActivityResult(int requestCode, int resultCode, Intent data)
	{
		return mHelper.handleActivityResult(requestCode, resultCode, data);
	}
}
