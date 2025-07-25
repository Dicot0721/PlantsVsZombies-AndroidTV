package com.transmension.mobile;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputFilter;
import android.text.ParcelableSpan;
import android.text.Selection;
import android.text.SpanWatcher;
import android.text.Spannable;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.CompletionInfo;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.ExtractedText;
import android.view.inputmethod.ExtractedTextRequest;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

/* loaded from: classes.dex */
public class AndroidTextInputManager implements TextInputManager {
    static final boolean DEBUG = false;
    static final int EXTRACT_NOTHING = -2;
    static final int EXTRACT_UNKNOWN = -1;
    static final int IME_FLAG_NAVIGATE_NEXT = 134217728;
    static final String TAG = "TextInputManager";
    private static final int PRIORITY = 100;
    private final InputMethodManager mIMM;
    private final String mHint = "";
    private final ExtractedText mExtractedText = new ExtractedText();
    private final Editable.Factory mEditableFactory = Editable.Factory.getInstance();
    protected Context mContext;
    protected TextInputManager.Listener mListener;
    protected View mView;
    private ChangeWatcher mChangeWatcher;
    private ExtractedTextRequest mExtractedTextRequest;
    private EditorInfo mLastEditorInfo;
    private EditText mTextInput;
    private AlertDialog mTextInputDialog;
    private int mInputType = 1;
    private int mImeOptions = 0;
    private CharSequence mText = "";
    private int mBatchEditNesting = 0;
    private boolean mContentChanged = false;
    private int mChangedStart = -1;
    private int mChangedEnd = -1;
    private int mChangedDelta = 0;
    private boolean mSelectionModeChanged = false;
    private long mInputCookie = 0;
    private long mLastInputCookie = 0;

    public AndroidTextInputManager(Context context, View view) {
        this.mContext = context;
        this.mIMM = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
        this.mView = view;
        setText("");
    }

    private static boolean isMultilineInputType(int type) {
        return (131087 & type) == 131073;
    }

    @Override // com.transmension.mobile.TextInputManager
    public long getInputCookie() {
        return this.mInputCookie;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setInputCookie(long cookie) {
        Log.i(TAG, String.format("setInputCookie: 0x%x", cookie));
        this.mInputCookie = cookie;
    }

    @Override // com.transmension.mobile.TextInputManager
    public int getInputType() {
        return this.mInputType;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setInputType(int type) {
        this.mInputType = type;
    }

    @Override // com.transmension.mobile.TextInputManager
    public int getImeOptions() {
        return this.mImeOptions;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setImeOptions(int options) {
        this.mImeOptions = options;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setText(String text, int selectionStart, int selectionEnd) {
        Log.i(TAG, "setText: " + text + " " + selectionStart + ":" + selectionEnd);
        boolean textChanged = !getText().toString().equals(text);
        boolean selectionChanged = selectionStart >= 0 && selectionEnd >= 0 && !(selectionStart == getSelectionStart() && selectionEnd == getSelectionEnd());
        if (!textChanged && !selectionChanged) {
            if (this.mLastEditorInfo == null || this.mLastEditorInfo.inputType != getInputType() || this.mLastEditorInfo.imeOptions != getImeOptions() || !this.mLastEditorInfo.hintText.toString().equals(this.mHint)) {
                InputMethodManager imm = this.mIMM;
                if (imm != null) {
                    imm.restartInput(this.mView);
                }
                reportChanges();
                return;
            }
            Log.i(TAG, "Ignored the redundant text change.");
            return;
        }
        Log.i(TAG, "current: " + getText() + " " + getSelectionStart() + ":" + getSelectionEnd());
        if (textChanged && (selectionStart == -1 || selectionEnd == -1)) {
            selectionStart = getSelectionStart();
            selectionEnd = getSelectionEnd();
        }
        this.mText = this.mEditableFactory.newEditable(text);
        int textLength = text.length();
        if (this.mText != null) {
            Spannable sp = (Spannable) this.mText;
            ChangeWatcher[] watchers = sp.getSpans(0, sp.length(), ChangeWatcher.class);
            for (ChangeWatcher changeWatcher : watchers) {
                sp.removeSpan(changeWatcher);
            }
            if (this.mChangeWatcher == null) {
                this.mChangeWatcher = new ChangeWatcher(this, null);
            }
            sp.setSpan(this.mChangeWatcher, 0, textLength, (PRIORITY << 16) | 18);
        }
        if (selectionEnd > text.length()) {
            selectionEnd = text.length();
        }
        if (selectionStart > selectionEnd) {
            selectionStart = selectionEnd;
        }
        if (selectionStart != -1 && selectionEnd != -1) {
            Selection.setSelection((Spannable) this.mText, selectionStart, selectionEnd);
            this.mSelectionModeChanged = selectionChanged;
        }
        InputMethodManager imm2 = this.mIMM;
        if (imm2 != null) {
            imm2.restartInput(this.mView);
        }
        reportChanges();
    }

    @Override // com.transmension.mobile.TextInputManager
    public CharSequence getText() {
        return this.mText;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setText(String text) {
        Log.i(TAG, "setText: " + text);
        this.mText = this.mEditableFactory.newEditable(text);
        InputMethodManager imm = this.mIMM;
        if (imm != null) {
            imm.restartInput(this.mView);
        }
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setSelection(int start, int end) {
        Log.i(TAG, "setSelection: " + start + ":" + end);
        if (start <= this.mText.length() && end <= this.mText.length()) {
            if (start != getSelectionStart() || end != getSelectionEnd()) {
                Selection.setSelection((Spannable) this.mText, start, end);
                this.mSelectionModeChanged = true;
                reportChanges();
            }
        }
    }

    @Override // com.transmension.mobile.TextInputManager
    public int getSelectionStart() {
        return Selection.getSelectionStart(this.mText);
    }

    @Override // com.transmension.mobile.TextInputManager
    public int getSelectionEnd() {
        return Selection.getSelectionEnd(this.mText);
    }

    public boolean isSingleLine() {
        return !isMultilineInputType(getInputType());
    }

    public Editable getEditableText() {
        if (this.mText instanceof Editable) {
            return (Editable) this.mText;
        }
        return null;
    }

    public void onBeginBatchEdit(long cookie) {
    }

    public void onEndBatchEdit(long cookie) {
        int start = Selection.getSelectionStart(this.mText);
        int end = Selection.getSelectionEnd(this.mText);
        Log.i(TAG, "Text: " + this.mText + " Selection: " + start + ":" + end);
        onTextChanged(this.mText.toString(), start, end, cookie);
    }

    public void beginBatchEdit(long cookie) {
        Log.d(TAG, "beginBatchEdit()");
        int nesting = this.mBatchEditNesting + 1;
        this.mBatchEditNesting = nesting;
        if (nesting == 1) {
            if (this.mContentChanged) {
                this.mChangedStart = 0;
                this.mChangedEnd = getText().length();
            } else {
                this.mChangedStart = -1;
                this.mChangedEnd = -1;
            }
            onBeginBatchEdit(cookie);
        }
    }

    public void endBatchEdit(long cookie) {
        Log.d(TAG, "endBatchEdit()");
        int nesting = this.mBatchEditNesting - 1;
        this.mBatchEditNesting = nesting;
        if (nesting == 0) {
            finishBatchEdit(cookie);
        }
    }

    void ensureEndedBatchEdit(long cookie) {
        if (this.mBatchEditNesting != 0) {
            this.mBatchEditNesting = 0;
            finishBatchEdit(cookie);
        }
    }

    void finishBatchEdit(long cookie) {
        Log.d(TAG, "finishBatchEdit()");
        onEndBatchEdit(cookie);
        if (this.mContentChanged || this.mSelectionModeChanged) {
            reportExtractedText();
        }
    }

    boolean reportExtractedText() {
        InputMethodManager imm;
        Log.i(TAG, "reportExtractedText(): " + this.mContentChanged);
        boolean contentChanged = this.mContentChanged;
        if (contentChanged || this.mSelectionModeChanged) {
            this.mContentChanged = false;
            this.mSelectionModeChanged = false;
            ExtractedTextRequest req = this.mExtractedTextRequest;
            if (req != null && (imm = this.mIMM) != null) {
                Log.v(TAG, "Retrieving extracted start=" + this.mChangedStart + " end=" + this.mChangedEnd + " delta=" + this.mChangedDelta);
                if (this.mChangedStart < 0 && !contentChanged) {
                    this.mChangedStart = -2;
                }
                if (extractText(req, this.mChangedStart, this.mChangedEnd, this.mChangedDelta, this.mExtractedText)) {
                    Log.v(TAG, "Reporting extracted start=" + this.mExtractedText.partialStartOffset + " end=" + this.mExtractedText.partialEndOffset + ": " + this.mExtractedText.text);
                    imm.updateExtractedText(this.mView, req.token, this.mExtractedText);
                    this.mChangedStart = -1;
                    this.mChangedEnd = -1;
                    this.mChangedDelta = 0;
                    this.mContentChanged = false;
                    this.mSelectionModeChanged = false;
                    return true;
                }
            }
        }
        return false;
    }

    public void onCommitCompletion(CompletionInfo text) {
        Log.d(TAG, "onCommitCompletion(): " + text);
    }

    public boolean extractText(ExtractedTextRequest request, ExtractedText outText) {
        return extractText(request, -1, -1, -1, outText);
    }

    public boolean extractText(ExtractedTextRequest request, int partialStartOffset, int partialEndOffset, int delta, ExtractedText outText) {
        int partialEndOffset2;
        CharSequence content = this.mText;
        if (content != null) {
            if (partialStartOffset != -2) {
                int N = content.length();
                if (partialStartOffset < 0) {
                    outText.partialEndOffset = -1;
                    outText.partialStartOffset = -1;
                    partialStartOffset = 0;
                    partialEndOffset2 = N;
                } else {
                    partialEndOffset2 = partialEndOffset + delta;
                    if (content instanceof Spanned) {
                        Spanned spanned = (Spanned) content;
                        Object[] spans = spanned.getSpans(partialStartOffset, partialEndOffset2, ParcelableSpan.class);
                        int i = spans.length;
                        while (i > 0) {
                            i--;
                            int j = spanned.getSpanStart(spans[i]);
                            if (j < partialStartOffset) {
                                partialStartOffset = j;
                            }
                            int j2 = spanned.getSpanEnd(spans[i]);
                            if (j2 > partialEndOffset2) {
                                partialEndOffset2 = j2;
                            }
                        }
                    }
                    outText.partialStartOffset = partialStartOffset;
                    outText.partialEndOffset = partialEndOffset2 - delta;
                    if (partialStartOffset > N) {
                        partialStartOffset = N;
                    } else if (partialStartOffset < 0) {
                        partialStartOffset = 0;
                    }
                    if (partialEndOffset2 > N) {
                        partialEndOffset2 = N;
                    } else if (partialEndOffset2 < 0) {
                        partialEndOffset2 = 0;
                    }
                }
                if ((request.flags & 1) != 0) {
                    outText.text = content.subSequence(partialStartOffset, partialEndOffset2);
                } else {
                    outText.text = TextUtils.substring(content, partialStartOffset, partialEndOffset2);
                }
            } else {
                outText.partialStartOffset = 0;
                outText.partialEndOffset = 0;
                outText.text = "";
            }
            outText.flags = 0;
            if (isSingleLine()) {
                outText.flags |= 1;
            }
            outText.startOffset = 0;
            outText.selectionStart = getSelectionStart();
            outText.selectionEnd = getSelectionEnd();
            return true;
        }
        return false;
    }

    public void setExtracting(ExtractedTextRequest req) {
        this.mExtractedTextRequest = req;
    }

    void reportChanges() {
        InputMethodManager imm;
        Log.i(TAG, "reportChanges()");
        int selectionStart = getSelectionStart();
        int selectionEnd = getSelectionEnd();
        if (this.mBatchEditNesting == 0 && (imm = this.mIMM) != null && imm.isActive(this.mView)) {
            boolean reported = false;
            if (this.mContentChanged || this.mSelectionModeChanged) {
                reported = reportExtractedText();
            }
            if (!reported) {
                Log.i(TAG, String.format("updateSelection: %d:%d %d:%d", selectionStart, selectionEnd, -1, -1));
                imm.updateSelection(this.mView, selectionStart, selectionEnd, -1, -1);
            }
        }
    }

    void reportCursor(int left, int top, int right, int bottom) {
        InputMethodManager imm = this.mIMM;
        if (imm != null && imm.isActive(this.mView) && imm.isWatchingCursor(this.mView)) {
            imm.updateCursor(this.mView, left, top, right, bottom);
        }
    }

    public void onEditorAction(int actionCode) {
        if (this.mListener != null) {
            this.mListener.onEditorAction(actionCode);
        }
    }

    void handleTextChanged(CharSequence buffer, int start, int before, int after) {
        this.mContentChanged = true;
        if (this.mChangedStart < 0) {
            this.mChangedStart = start;
            this.mChangedEnd = start + before;
        } else {
            if (this.mChangedStart > start) {
                this.mChangedStart = start;
            }
            if (this.mChangedEnd < start + before) {
                this.mChangedEnd = start + before;
            }
        }
        this.mChangedDelta += after - before;
    }

    void spanChange(Spanned buf, Object what, int oldStart, int newStart, int oldEnd, int newEnd) {
        boolean selChanged = false;
        int newSelStart = -1;
        int newSelEnd = -1;
        if (what == Selection.SELECTION_END) {
            selChanged = true;
            newSelEnd = newStart;
        }
        if (what == Selection.SELECTION_START) {
            selChanged = true;
            newSelStart = newStart;
        }
        if (selChanged && (buf.getSpanFlags(what) & 512) == 0) {
            if (newSelStart < 0) {
                Selection.getSelectionStart(buf);
            }
            if (newSelEnd < 0) {
                Selection.getSelectionEnd(buf);
            }
            if (this.mBatchEditNesting == 0) {
                onEndBatchEdit(this.mLastInputCookie);
            }
        }
        if ((what instanceof ParcelableSpan) && this.mExtractedTextRequest != null) {
            if (this.mBatchEditNesting != 0) {
                if (oldStart >= 0) {
                    if (this.mChangedStart > oldStart) {
                        this.mChangedStart = oldStart;
                    }
                    if (this.mChangedStart > oldEnd) {
                        this.mChangedStart = oldEnd;
                    }
                }
                if (newStart >= 0) {
                    if (this.mChangedStart > newStart) {
                        this.mChangedStart = newStart;
                    }
                    if (this.mChangedStart > newEnd) {
                        this.mChangedStart = newEnd;
                        return;
                    }
                    return;
                }
                return;
            }
            this.mContentChanged = true;
        }
    }

    @Override // com.transmension.mobile.TextInputManager
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        Log.i(TAG, "onCreateInputConnection()");
        outAttrs.inputType = getInputType();
        outAttrs.imeOptions = this.mImeOptions;
        outAttrs.privateImeOptions = "";
        outAttrs.actionLabel = null;
        outAttrs.actionId = 0;
        outAttrs.extras = new Bundle();
        if ((outAttrs.imeOptions & 255) == 0) {
            if ((outAttrs.imeOptions & IME_FLAG_NAVIGATE_NEXT) != 0) {
                outAttrs.imeOptions |= 5;
            } else {
                outAttrs.imeOptions |= 6;
            }
        }
        if (isMultilineInputType(outAttrs.inputType)) {
            outAttrs.imeOptions |= 1073741824;
        }
        outAttrs.hintText = this.mHint;
        InputConnection ic = new EditableInputConnection(this, this.mInputCookie);
        outAttrs.initialSelStart = getSelectionStart();
        outAttrs.initialSelEnd = getSelectionEnd();
        outAttrs.initialCapsMode = ic.getCursorCapsMode(getInputType());
        this.mLastEditorInfo = outAttrs;
        this.mLastInputCookie = this.mInputCookie;
        return ic;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void showIme(int mode) {
        if (this.mIMM != null) {
            this.mIMM.showSoftInput(this.mView, mode);
        }
    }

    @Override // com.transmension.mobile.TextInputManager
    public void hideIme(int mode) {
        if (this.mIMM != null) {
            this.mIMM.hideSoftInputFromWindow(this.mView.getWindowToken(), mode);
        }
    }

    @Override // com.transmension.mobile.TextInputManager
    public void showTextInputDialog(int mode, String title, String hint, String initial) {
        if (this.mTextInputDialog != null) {
            this.mTextInput.setHint(hint);
            this.mTextInput.setText(initial);
            this.mTextInputDialog.show();
            return;
        }
        this.mTextInput = new EditText(this.mContext);
        int type = 1;
        switch (mode) {
            case 0:
                break;
            case 1:
                type = 129;
                break;
            case 2:
                type = 17;
                break;
            case 3:
                type = 33;
                break;
            case 4:
                type = 145;
                break;
        }
        this.mTextInput.setInputType(type);
        if (mode == 1) {
            this.mTextInput.setTransformationMethod(PasswordTransformationMethod.getInstance());
        }
        if (mode == 4 || mode == 1) {
            InputFilter filter = (source, start, end, dest, dstart, dend) -> {
                for (int i = start; i < end; i++) {
                    if (!Character.isLetterOrDigit(source.charAt(i)) || source.charAt(i) > '\u007f') {
                        return "";
                    }
                }
                return null;
            };
            this.mTextInput.setFilters(new InputFilter[]{filter});
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(this.mContext);
        this.mTextInput.setHint(hint);
        this.mTextInput.setText(initial);
        builder.setTitle(title);
        builder.setView(this.mTextInput);
        builder.setPositiveButton("确定", (dialogInterface, i) -> onTextInput(mTextInput.getText().toString()));
        builder.setNegativeButton("返回", (dialogInterface, i) -> {
            onTextInput(null);
            hideTextInputDialog(true);
        });
        builder.setOnCancelListener(dialogInterface -> {
            onTextInput(null);
            hideTextInputDialog(true);
        });
        this.mTextInputDialog = builder.create();
        this.mTextInputDialog.show();
    }

    @Override // com.transmension.mobile.TextInputManager
    public void hideTextInputDialog() {
        hideTextInputDialog(false);
    }

    @Override // com.transmension.mobile.TextInputManager
    public void hideTextInputDialog(boolean cancel) {
        if (this.mTextInputDialog != null) {
            DialogInterface dialog = this.mTextInputDialog;
            this.mTextInputDialog = null;
            this.mTextInput = null;
            if (cancel) {
                dialog.cancel();
            } else {
                dialog.dismiss();
            }
        }
    }

    @Override // com.transmension.mobile.TextInputManager
    public TextInputManager.Listener getListener() {
        return this.mListener;
    }

    @Override // com.transmension.mobile.TextInputManager
    public void setListener(TextInputManager.Listener listener) {
        this.mListener = listener;
    }

    void onTextChanged(String text, int start, int end, long cookie) {
        if (this.mListener != null) {
            this.mListener.onTextChanged(this.mView, text, start, end, cookie);
        }
    }

    void onTextInput(String text) {
        if (this.mListener != null) {
            this.mListener.onTextInput(this.mView, text);
        }
    }

    /* loaded from: classes.dex */
    public static class EditableInputConnection extends BaseInputConnection {
        private static final boolean DEBUG = true;
        private static final String TAG = "EditableInputConnection";
        private final long mCookie;
        private final AndroidTextInputManager mTextInput;
        private int mBatchEditNesting;

        public EditableInputConnection(AndroidTextInputManager textInput, long cookie) {
            super(textInput.mView, true);
            this.mTextInput = textInput;
            this.mCookie = cookie;
        }

        public long getCookie() {
            return this.mCookie;
        }

        @Override // android.view.inputmethod.BaseInputConnection
        public Editable getEditable() {
            AndroidTextInputManager tv = this.mTextInput;
            if (tv != null) {
                return tv.getEditableText();
            }
            return null;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean beginBatchEdit() {
            synchronized (this) {
                if (this.mBatchEditNesting >= 0) {
                    this.mTextInput.beginBatchEdit(this.mCookie);
                    this.mBatchEditNesting++;
                    return true;
                }
                return false;
            }
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean endBatchEdit() {
            synchronized (this) {
                if (this.mBatchEditNesting > 0) {
                    this.mTextInput.endBatchEdit(this.mCookie);
                    this.mBatchEditNesting--;
                    return true;
                }
                return false;
            }
        }

        protected void reportFinish() {
            Log.v(TAG, "reportFinish()");
            synchronized (this) {
                while (this.mBatchEditNesting > 0) {
                    endBatchEdit();
                }
                this.mBatchEditNesting = -1;
            }
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean clearMetaKeyStates(int states) {
            Editable content = getEditable();
            return content != null;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean commitCompletion(CompletionInfo text) {
            Log.v(TAG, "commitCompletion " + text);
            this.mTextInput.beginBatchEdit(this.mCookie);
            this.mTextInput.onCommitCompletion(text);
            this.mTextInput.endBatchEdit(this.mCookie);
            return true;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean performEditorAction(int actionCode) {
            Log.v(TAG, "performEditorAction " + actionCode);
            this.mTextInput.onEditorAction(actionCode);
            return true;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean performContextMenuAction(int id) {
            Log.v(TAG, "performContextMenuAction " + id);
            this.mTextInput.beginBatchEdit(this.mCookie);
            this.mTextInput.endBatchEdit(this.mCookie);
            return true;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public ExtractedText getExtractedText(ExtractedTextRequest request, int flags) {
            Log.v(TAG, "getExtractedText " + request + " with " + flags);
            if (this.mTextInput != null) {
                ExtractedText et = new ExtractedText();
                if (this.mTextInput.extractText(request, et)) {
                    if ((flags & 1) != 0) {
                        this.mTextInput.setExtracting(request);
                        return et;
                    }
                    return et;
                }
            }
            return null;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean performPrivateCommand(String action, Bundle data) {
            return true;
        }

        @Override // android.view.inputmethod.BaseInputConnection, android.view.inputmethod.InputConnection
        public boolean commitText(CharSequence text, int newCursorPosition) {
            Log.i(TAG, "commitText: " + text + " at " + newCursorPosition);
            return super.commitText(text, newCursorPosition);
        }
    }

    /* loaded from: classes.dex */
    private class ChangeWatcher implements TextWatcher, SpanWatcher {
        private ChangeWatcher() {
        }

        /* synthetic */ ChangeWatcher(AndroidTextInputManager androidTextInputManager, ChangeWatcher changeWatcher) {
            this();
        }

        @Override // android.text.TextWatcher
        public void beforeTextChanged(CharSequence buffer, int start, int before, int after) {
        }

        @Override // android.text.TextWatcher
        public void onTextChanged(CharSequence buffer, int start, int before, int after) {
            AndroidTextInputManager.this.handleTextChanged(buffer, start, before, after);
        }

        @Override // android.text.TextWatcher
        public void afterTextChanged(Editable buffer) {
        }

        @Override // android.text.SpanWatcher
        public void onSpanChanged(Spannable buf, Object what, int s, int e, int st, int en) {
            AndroidTextInputManager.this.spanChange(buf, what, s, st, e, en);
        }

        @Override // android.text.SpanWatcher
        public void onSpanAdded(Spannable buf, Object what, int s, int e) {
            AndroidTextInputManager.this.spanChange(buf, what, -1, s, -1, e);
        }

        @Override // android.text.SpanWatcher
        public void onSpanRemoved(Spannable buf, Object what, int s, int e) {
            AndroidTextInputManager.this.spanChange(buf, what, s, -1, e, -1);
        }
    }
}