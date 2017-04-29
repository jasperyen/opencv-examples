/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package captchadetection;

import org.opencv.core.Core;

/**
 *
 * @author Jasper
 */
public class CaptchaDetection {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
    }
    
}
