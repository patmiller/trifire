//
//  ViewController.swift
//  Trifire
//
//  Created by Patrick Miller on 6/30/25.
//

import UIKit
import AudioToolbox
import AVFoundation


class ViewController: UIViewController {

    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    @IBOutlet weak var spinButton: UIButton!
    @IBOutlet weak var fireButton: UIButton!
    @IBOutlet weak var gamescreen: UIImageView!
    @IBOutlet weak var turnLabel: UILabel!
    @IBOutlet weak var scoreLabel: UILabel!
    
    // Probably a way to statically allocate these
    let width = 640
    let height = 480
    var state = UnsafeMutablePointer<Int>.allocate(capacity: 8)
    let arena = UnsafeMutablePointer<Int32>.allocate(capacity: 640*480);
    
    var audioPlayer: AVAudioPlayer?
    
    func boom() {
        print("try to boom")
        guard let url = Bundle.main.url(forResource: "boom", withExtension: "mp3") else {
            print("Sound file not found")
            return
        }
        print(url)
        
        do {
            print("update audioPlayer")
            audioPlayer = try AVAudioPlayer(contentsOf: url)
            print("set volume")
            audioPlayer?.volume = 1.0
            print("prepare")
            audioPlayer?.prepareToPlay()
            print("play")
            audioPlayer?.play()
        } catch {
            print("Error loading sound: \(error)")
        }
        print("done with boom")
    }

    func play(command: Character) {
        if let scalar = command.unicodeScalars.first {
            print("command",command, scalar.value)
            
            let hit = playlong(Int8(scalar.value),state)
            print("play",hit)
            print("render",renderlong(arena, UInt32(width), UInt32(height), state))
            let turn = state[0]
            // We want to detect the explosion when we render, not when we play next turn
            var score = state[1]
            if hit == 2 {
                score += 1
                boom()
            }
            print("turn", turn, "score", score)
            
            // Create pixel buffer (RGBA8888)
            let bytesPerPixel = 4
            let bytesPerRow = bytesPerPixel * width
            
            // Create CGContext
            let colorSpace = CGColorSpaceCreateDeviceRGB()
            let context = CGContext(
                data: arena,
                width: width,
                height: height,
                bitsPerComponent: 8,
                bytesPerRow: bytesPerRow,
                space: colorSpace,
                bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue
            )
            
            // Create CGImage
            DispatchQueue.main.async {
                self.turnLabel.text = "\(turn)"
                self.scoreLabel.text = "\(score)"
                if let cgImage = context?.makeImage() {
                    let image = UIImage(cgImage: cgImage)
                    self.gamescreen.image = image
                }
            }
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        //Save stuff to user data on exit
        NotificationCenter.default.addObserver(
            self,
            selector: #selector(saveData),
            name: UIApplication.willResignActiveNotification,
            object: nil
        )
        
        if let savedArray = UserDefaults.standard.array(forKey: "state") as? [Int] {
            print(savedArray)
            for i in 0..<8 {
                state[i] = savedArray[i]
            }
        } else {
            print("No data found.")
        }
        
        do {
            try AVAudioSession.sharedInstance().setCategory(.playback, mode: .default)
            try AVAudioSession.sharedInstance().setActive(true)
        } catch {
            print("Failed to set audio session: \(error)")
        }
        
        leftButton.setImage(UIImage(named: "left_button"), for: .normal)
        rightButton.setImage(UIImage(named: "right_button"), for: .normal)
        spinButton.setImage(UIImage(named: "spin_button"), for: .normal)
        fireButton.setImage(UIImage(named: "fire_button"), for: .normal)
        
        turnLabel.text = "0"
        scoreLabel.text = "0"

        let queue = OperationQueue()
        queue.maxConcurrentOperationCount = 1 // serial queue
        queue.addOperation {
            self.play(command: "X")  // Just renders turn 0, X is not a command
        }
        
        let leftAction = UIAction { _ in
            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "l")
            }
        }
        leftButton.addAction(leftAction, for: .touchUpInside)
        
        
        let rightAction = UIAction { _ in
            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "r")
            }
        }
        rightButton.addAction(rightAction, for: .touchUpInside)
        
        
        let spinAction = UIAction { _ in
            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "s")
            }
        }
        spinButton.addAction(spinAction, for: .touchUpInside)
        
        
        let fireAction = UIAction { _ in
            // We need to serialize the play and render since the C code is not thread safe
            queue.addOperation {
                self.play(command: "f")
            }
        }
        fireButton.addAction(fireAction, for: .touchUpInside)

    }



    @objc func saveData() {
        // Make an array with our state
        let array = Array(UnsafeBufferPointer(start: state, count: 8))
        print("save",array)
        UserDefaults.standard.set(array, forKey: "state")
    }

}

